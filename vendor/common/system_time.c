/********************************************************************************************************
 * @file	system_time.c
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
#include "tl_common.h"
#include "proj_lib/ble/blt_config.h"
#include "vendor/common/user_config.h"
#include "app_health.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "vendor/common/time_model.h"
#include "time_model.h"
#include "lighting_model_LC.h"
#include "sensors_model.h"
#include "system_time.h"
#include "fast_provision_model.h"
#include "app_heartbeat.h"
#include "directed_forwarding.h"
#if PAIR_PROVISION_ENABLE
#include "pair_provision.h"
#endif

#if ALI_MD_TIME_EN
#include "user_ali_time.h"
#endif

#ifndef __PROJECT_MESH_SWITCH__
#define __PROJECT_MESH_SWITCH__     0
#endif

#if GW_SMART_PROVISION_REMOTE_CONTROL_PM_EN
#define MESH_LONG_SLEEP_WAKEUP_EN	1
#endif

#ifndef MESH_LONG_SLEEP_WAKEUP_EN
#define MESH_LONG_SLEEP_WAKEUP_EN	1   // for switch project, no need to enable and just use API of cpu_long_sleep_wakeup_() is ok.
#endif

#if WIN32 // just for compile
u32 flash_sector_mac_address = 0x76000;
u32 flash_sector_calibration = 0x77000;
#endif

u32 system_time_ms = 0;
u32 system_time_100ms = 0;
u32 system_time_s = 0;
u32 system_time_tick;

#ifndef RTC_USE_32K_RC_ENABLE
/* if PM enable, it is about 120ppm(10s/day) at 25 degrees Celsius after enable RTC_USE_32K_RC_ENABLE.*/
#define RTC_USE_32K_RC_ENABLE	0		// 1.Don't enable if no sleep application. 2. when there is sleep application, RTC will be more accurate with being enable this function.
#endif

#if __PROJECT_MESH_SWITCH__
#define CHECK_INTERVAL      (1*1000*32)  // 32k tick of 1s
#elif MESH_LONG_SLEEP_WAKEUP_EN
#define CHECK_INTERVAL      (32)  		 // 32k tick of 1ms
#elif RTC_USE_32K_RC_ENABLE
#define CHECK_INTERVAL      (500*CLOCK_16M_SYS_TIMER_CLK_1MS)
#else
#define CHECK_INTERVAL      (1 * CLOCK_SYS_CLOCK_1MS) // must 1ms, because light_transition_proc() need 1ms tick.
#endif

#define RTC_LEFT_MS 	(system_time_ms%1000+(tick_32k-tick_32k_begin)/32)
#define LOG_RTC_DEBUG(pbuf, len, format, ...)		//LOG_MSG_LIB(TL_LOG_NODE_BASIC, pbuf, len, format, ##__VA_ARGS__)

#if (__PROJECT_MESH_SWITCH__ || RTC_USE_32K_RC_ENABLE)
STATIC_ASSERT(MESH_LONG_SLEEP_WAKEUP_EN == 0); // no need to enable MESH_LONG_SLEEP_WAKEUP_EN and just use API of cpu_long_sleep_wakeup_() is ok. // because switch has enable long sleep mode
#endif

#if RTC_USE_32K_RC_ENABLE
u32 cal_unit_32k;
u32 cal_unit_16m;
u32 tick_16m_begin;
u32 tick_32k_begin;
u32 tick_32k_to_16m = 0;
u8 rtc_adjust_flag = 0;
_attribute_ram_code_  void read_tick_32k_16m(u8 *tick_32k, u32 * tick_16m)
{
	u32 r=irq_disable();
	u8 pre = analog_read(0x40);
	while( pre == analog_read(0x40) );
	(*tick_16m) = clock_time();			
	tick_32k[0] = analog_read(0x40);
	tick_32k[1] = analog_read(0x41);
	tick_32k[2] = analog_read(0x42);
	tick_32k[3] = analog_read(0x43);
	irq_restore(r);
	return;
}

void rtc_cal_init(u8 tick_start)
{
	u32 tmp_32k_1, tmp_32k_2;
	u32 tmp_16m_1, tmp_16m_2;
	read_tick_32k_16m((u8 *)&tmp_32k_1, (u32 *)&tmp_16m_1);
	sleep_us(512*1000);
	read_tick_32k_16m((u8 *)&tmp_32k_2, (u32 *)&tmp_16m_2);

	cal_unit_32k = tmp_32k_2 - tmp_32k_1;
	cal_unit_16m = tmp_16m_2 - tmp_16m_1;
	
	if(tick_start){
		tick_32k_begin = tmp_32k_1;
		tick_16m_begin = tick_32k_to_16m = tmp_16m_1;
	}
	LOG_RTC_DEBUG(0, 0, "rtc_cal_init");
}
#endif

void system_timer_handle_ms()
{
}

void system_timer_handle_100ms()
{
	mesh_heartbeat_poll_100ms();
#if ALI_MD_TIME_EN
	user_ali_time_proc();
#endif
#if (!WIN32 && SENSOR_LIGHTING_CTRL_USER_MODE_EN && MD_SENSOR_CLIENT_EN)
    sensor_lighting_ctrl_proc();
#endif
#if (MD_DF_CFG_SERVER_EN && !WIN32 && !FEATURE_LOWPOWER_EN)
	mesh_directed_forwarding_proc(0, 0, 0, MESH_BEAR_ADV);
#endif
}

static inline u32 get_tick_for_system_time()
{
#if (__PROJECT_MESH_SWITCH__ || MESH_LONG_SLEEP_WAKEUP_EN)
	return get_32k_tick();
#else
	return clock_time();
#endif
}

void system_time_init(){
	system_time_tick = get_tick_for_system_time();
#if RTC_USE_32K_RC_ENABLE
	LOG_RTC_DEBUG(0, 0, "system_time_init");
	rtc_cal_init(1);							
#endif	
}

#if (FEATURE_LOWPOWER_EN || GATT_LPN_EN)
_attribute_ram_code_
#endif
void system_time_run(){
    mesh_iv_update_start_poll();
    
    u32 clock_tmp = get_tick_for_system_time();
    u32 t_delta = clock_tmp - system_time_tick;	
	#if RTC_USE_32K_RC_ENABLE
	if(t_delta < BIT(31))
	#else
    if(t_delta >= CHECK_INTERVAL)
	#endif
	{
		#if RTC_USE_32K_RC_ENABLE 
		u32 tick_32k, tick_16m;
		read_tick_32k_16m((u8 *)&tick_32k, (u32 *)&tick_16m);
		u32 unit_cnt = (u32)(tick_32k - tick_32k_begin)/cal_unit_32k;
		tick_32k_begin += unit_cnt*cal_unit_32k;
		tick_16m_begin += unit_cnt*cal_unit_16m;
		u32 delta = tick_16m_begin - tick_32k_to_16m;
		if(delta > BIT(31)){
			delta = 0; // not necessary. due to have been sure not happen here. // skip this round and wait until (tick_16m_begin > tick_last).
		}
		
		u32 interval_cnt = delta/CLOCK_SYS_CLOCK_1MS;
		if(interval_cnt){		
			tick_32k_to_16m += interval_cnt*CLOCK_SYS_CLOCK_1MS;
			if(rtc_adjust_flag){ // Note: if no sleep for a long time, no need compentation.
				rtc_adjust_flag = 0;
				interval_cnt -= 13;// minus 13ms per minute				
			}
		}
		#else
        u32 interval_cnt = t_delta/CHECK_INTERVAL;
		#endif
        if(interval_cnt){
			#if __PROJECT_MESH_SWITCH__
			system_time_s += interval_cnt;
			mesh_iv_update_st_poll_s(interval_cnt);
			LOG_RTC_DEBUG(0, 0, "%02d:%02d:%02d", system_time_s/60/60, (system_time_s/60)%60, system_time_s%60);					
			#else
            u32 inc_100ms = (system_time_ms % 100 + interval_cnt) / 100;

            	#if LLSYNC_ENABLE
			llsync_mesh_timer_period_proc(system_time_ms, interval_cnt); // must before "system_time_ms +="
            	#endif
			system_time_ms += interval_cnt;
						
			if(!(RTC_USE_32K_RC_ENABLE || FEATURE_LOWPOWER_EN || SPIRIT_PRIVATE_LPN_EN || GATT_LPN_EN ||DU_LPN_EN || MESH_LONG_SLEEP_WAKEUP_EN)){ // (!is_lpn_support_and_en){ // it will cost several tens ms from wake up
                #if (PM_DEEPSLEEP_RETENTION_ENABLE)
                // no need to run system_timer_handle_ms_,
                // and it will take too much time to run system_timer_handle_ms_() for low power device which is wakeup after a long time sleep.
                #else
					#if MD_SERVER_EN
			    foreach(i,interval_cnt){
					if(0 == light_transition_proc()){
						break; // quick break to save time.
					}
				}
					#endif
					
			    foreach(i,interval_cnt){
			        system_timer_handle_ms();
			    }
			    #endif
			}
			
			if(inc_100ms){
                u32 inc_s = (system_time_100ms % 10 + inc_100ms) / 10;
    			system_time_100ms += inc_100ms;
				
                if(RTC_USE_32K_RC_ENABLE || is_lpn_support_and_en || SPIRIT_PRIVATE_LPN_EN ||DU_LPN_EN || MESH_LONG_SLEEP_WAKEUP_EN){ // it will cost several ms from wake up
                    system_timer_handle_100ms();	// only run once to save time
				}else{
					foreach(i,inc_100ms){
				        system_timer_handle_100ms();
				    }
				}
				
    			if(inc_s){		
					#if RTC_USE_32K_RC_ENABLE
					u32 now_s = system_time_s+inc_s;
					now_s = now_s;	// will be optimized.
					LOG_RTC_DEBUG(0, 0, "%02d:%02d:%02d.%03d", (now_s)/60/60, (now_s/60)%60, now_s%60+RTC_LEFT_MS/1000, RTC_LEFT_MS%1000);
					#endif
					foreach(i,inc_s){
						system_time_s++;
						#if VC_APP_ENABLE
						void sys_timer_refresh_time_ui();
						sys_timer_refresh_time_ui();
						#endif
						#if RTC_USE_32K_RC_ENABLE
						if((system_time_s%60) == 0){
							rtc_adjust_flag = 1;
							if((system_time_s%1800) == 0){
								rtc_cal_init(0);
							}
						}
						#endif
				    }				

					mesh_iv_update_st_poll_s(inc_s);
					user_system_time_proc(); // no need inside foreach to save time.
				    
				    //if(!is_lpn_support_and_en){
				        #if (!(__PROJECT_MESH_SWITCH__ || GW_SMART_PROVISION_REMOTE_CONTROL_PM_EN))
				        mesh_beacon_poll_1s();
				        #endif
					//}
    			}
            }
			#endif
		}
		#if RTC_USE_32K_RC_ENABLE 
		system_time_tick = clock_time() + CHECK_INTERVAL;
		#else
        system_time_tick += interval_cnt * CHECK_INTERVAL;
		#endif
		
        // proc handle 1ms or greater
	#if FAST_PROVISION_ENABLE
		mesh_fast_prov_proc();
	#endif
	#if PAIR_PROVISION_ENABLE
		pair_provision_proc();
	#endif
	#if ONLINE_STATUS_EN
        online_st_proc();
    #endif
    #if MD_SERVER_EN
        light_par_save_proc();
        #if MD_TIME_EN
        mesh_time_proc();
        #endif
	    #if (MD_LIGHT_CONTROL_EN)
        LC_property_proc();
	    #endif
	    #if (MD_SCENE_EN)
        scene_status_change_check_all();
	    #endif

        #if MD_SERVER_EN
	    if(publish_powerup_random_ms && clock_time_exceed_ms(0, publish_powerup_random_ms)){
	        publish_powerup_random_ms = 0;
			publish_when_powerup();
	    }
	    #endif
    #endif
    }

#if MD_SERVER_EN
    #if(MD_SENSOR_SERVER_EN)
//	sensor_measure_proc();
	#endif
#endif
}

u32 clock_time_exceed_ms(u32 ref, u32 span_ms){
	return ((u32)(clock_time_ms() - ref) > span_ms);
}

u32 clock_time_exceed_100ms(u32 ref, u32 span_100ms){
	return ((u32)(clock_time_100ms() - ref) > span_100ms);
}

u32 clock_time_exceed_s(u32 ref, u32 span_s){
	return ((u32)(clock_time_s() - ref) > span_s);
}

