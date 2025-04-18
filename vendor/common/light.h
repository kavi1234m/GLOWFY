/********************************************************************************************************
 * @file	light.h
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
#pragma once

#include "tl_common.h"
#include "vendor/common/mesh_node.h"

/** @addtogroup Mesh_Common
  * @{
  */
  
/** @defgroup Light
  * @brief Light Code.
  * @{
  */


#define LED_INDICATE_VAL    (rgb_lumen_map[100])
#define LED_INDICATE_LUM_VAL LED_INDICATE_VAL

#ifndef PWM_FREQ
#define PWM_FREQ	        (600)   // unit: Hz // PWM frequnce can be more than 1MHz if necessary.
#endif

#if __TLSR_RISCV_EN__
#define PWM_MAX_TICK        (PWM_PCLK_SPEED / PWM_FREQ)
#else
	#if(CLOCK_SYS_CLOCK_HZ == 48000000)
#define PWM_CLK_DIV_LIGHT   (1)
	#else
#define PWM_CLK_DIV_LIGHT   (0)
	#endif
#define PWM_MAX_TICK        ((CLOCK_SYS_CLOCK_HZ / (PWM_CLK_DIV_LIGHT + 1)) / PWM_FREQ)
#endif
//#define PMW_MAX_TICK		PWM_MAX_TICK

#if __TLSR_RISCV_EN__
static inline void pwm_start_id(pwm_id_e id)
{
	unsigned short bit = (PWM0_ID == id) ? BIT(8) : BIT(id);
	pwm_start(bit);
}

static inline void pwm_stop_id(pwm_id_e id)
{
	unsigned short bit = (PWM0_ID == id) ? BIT(8) : BIT(id);
	pwm_stop(bit);
}
#else
#define pwm_start_id(id)	pwm_start(id)
#define pwm_stop_id(id)		pwm_stop(id)
#endif

#define LED_MASK							0x07
#define	config_led_event(on,off,n,sel)		(on | (off<<8) | (n<<16) | (sel<<24))

#define	LED_EVENT_FLASH_4HZ_10S				config_led_event(2,2,40,LED_MASK)
#define	LED_EVENT_FLASH_4HZ_15S				config_led_event(2,2,60,LED_MASK)
#define	LED_EVENT_FLASH_STOP				config_led_event(1,1,1,LED_MASK)
#define	LED_EVENT_FLASH_2HZ_2S				config_led_event(4,4,4,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_1S				config_led_event(8,8,1,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_2S				config_led_event(8,8,2,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_3S				config_led_event(8,8,3,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_4S				config_led_event(8,8,4,LED_MASK)
#define	LED_EVENT_FLASH_4HZ					config_led_event(2,2,0,LED_MASK)
#define	LED_EVENT_FLASH_1HZ					config_led_event(8,8,0,LED_MASK)
#define	LED_EVENT_FLASH_4HZ_1T				config_led_event(2,2,1,LED_MASK)
#define	LED_EVENT_FLASH_4HZ_2T				config_led_event(2,2,2,LED_MASK)
#define	LED_EVENT_FLASH_4HZ_3T				config_led_event(2,2,3,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_1T				config_led_event(1,15,1,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_3T				config_led_event(8,8,3,LED_MASK)
#define	LED_EVENT_FLASH_2HZ_1T				config_led_event(4,4,1,LED_MASK)
#define	LED_EVENT_FLASH_2HZ_2T				config_led_event(4,4,2,LED_MASK)
#define	LED_EVENT_1SEC_ON_2SEC_OFF_30S		config_led_event(16,32,15,LED_MASK)
#define LED_EVENT_FLASH_1HZ_2TIMES          config_led_event(8,8,2*2,LED_MASK)
#define LED_EVENT_FLASH_OFF                config_led_event(0,1,1,LED_MASK)

#if HOMEKIT_EN
//////////////////////////////////////////////////////////////////////
//    LED
//////////////////////////////////////////////////////////////////////
#define LED_EVENT_FLASH_STOP                config_led_event(1,1,1,LED_MASK)

//#define LED_EVENT_FLASH_1HZ                 config_led_event(100,100,250,LED_MASK)
#define LED_EVENT_FLASH_1HZ_2TIMES          config_led_event(8,8,2*2,LED_MASK)//config_led_event(100,100,2*2,LED_MASK)
#define LED_EVENT_FLASH_1HZ_3TIMES          config_led_event(8,8,3*2,LED_MASK)
#define LED_EVENT_FLASH_1HZ_4TIMES          config_led_event(8,8,4*2,LED_MASK)
#define LED_EVENT_FLASH_1HZ_5TIMES          config_led_event(8,8,5*2,LED_MASK)
#define LED_EVENT_FLASH_1HZ_10TIMES         config_led_event(8,8,10*2,LED_MASK)
#define LED_EVENT_FLASH_1HZ_1_SECOND        config_led_event(8,8,1*2*1,LED_MASK)
#define LED_EVENT_FLASH_1HZ_2_SECOND        config_led_event(8,8,2*2*1,LED_MASK)
#define LED_EVENT_FLASH_1HZ_3_SECOND        config_led_event(8,8,3*2*1,LED_MASK)
#define LED_EVENT_FLASH_1HZ_4_SECOND        config_led_event(8,8,4*2*1,LED_MASK)
#define LED_EVENT_FLASH_1HZ_5_SECOND        config_led_event(8,8,5*2*1,LED_MASK)

#define LED_EVENT_FLASH_2HZ                 config_led_event(4,4,250,LED_MASK)
#define LED_EVENT_FLASH_2HZ_2TIMES          config_led_event(4,4,2*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_3TIMES          config_led_event(4,4,3*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_4TIMES          config_led_event(4,4,4*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_5TIMES          config_led_event(4,4,5*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_10TIMES         config_led_event(4,4,10*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_1_SECOND        config_led_event(4,4,1*2*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_2_SECOND        config_led_event(4,4,2*2*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_3_SECOND        config_led_event(4,4,3*2*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_4_SECOND        config_led_event(4,4,4*2*2,LED_MASK)
#define LED_EVENT_FLASH_2HZ_5_SECOND        config_led_event(4,4,5*2*2,LED_MASK)
#define	LED_EVENT_FLASH_1HZ_HK				config_led_event(8,8,250,LED_MASK)
#define	LED_EVENT_FLASH_4HZ_HK				config_led_event(2,2,250,LED_MASK)

//#define LED_EVENT_FLASH_4HZ                 config_led_event(25,25,250,LED_MASK)
#define LED_EVENT_FLASH_4HZ_5_SECOND        config_led_event(2,2,5*2*4,LED_MASK)

#define LED_EVENT_FLASH_5HZ                 config_led_event(2,2,250,LED_MASK)//config_led_event(20,20,250,LED_MASK)
#define LED_EVENT_FLASH_5HZ_3TIMES          config_led_event(2,2,3*2,LED_MASK)
#define LED_EVENT_FLASH_5HZ_5_SECOND        config_led_event(2,2,5*2*5,LED_MASK)

#endif

//#define LEVEL_MIN      			(-32767) // use "level off" instead.
//#define LEVEL_MAX      			(32767)
#if (0 == LIGHT_PAR_USER_EN)
#define LIGHTNESS_MIN      		(1)			// can not set 0
#define LIGHTNESS_MAX      		(0xFFFF)
#define CTL_TEMP_MIN			(0x0320)	// 800
#define CTL_TEMP_MAX			(0x4E20)	// 20000
#define CTL_D_UV_MIN			(-32768)
#define CTL_D_UV_MAX			(32767)
#define HSL_HUE_MIN				(0)
#define HSL_HUE_MAX				(0xFFFF)
#define HSL_SAT_MIN				(0)
#define HSL_SAT_MAX				(0xFFFF)
#define XYL_X_MIN				(0)
#define XYL_X_MAX				(0xFFFF)
#define XYL_Y_MIN				(0)
#define XYL_Y_MAX				(0xFFFF)

#define ONOFF_DEFAULT   	    (1)
#define LIGHTNESS_DEFAULT   	(LIGHTNESS_MAX)
#define CTL_TEMP_DEFAULT   		(CTL_TEMP_MAX)
#define CTL_D_UV_DEFAULT   		(0)
#define HSL_HUE_DEFAULT   		(HSL_HUE_MAX)
#define HSL_SAT_DEFAULT   		(HSL_SAT_MAX)
#define XYL_X_DEFAULT   		(XYL_X_MAX)
#define XYL_Y_DEFAULT   		(XYL_Y_MAX)
#endif

#define LEVEL_OFF				(-32768)	// don't change
#define LUM_OFF					(0)

#define HSL_HUE_CNT_TOTAL       (HSL_HUE_MAX + 1)
#define HSL_HUE_CNT_TOTAL_HALF  (HSL_HUE_CNT_TOTAL / 2)

#if (FEATURE_LOWPOWER_EN)
#define ONPOWER_UP_SELECT       ONPOWER_UP_DEFAULT
#elif (LIGHT_TYPE_SEL == TYPE_TOOTH_BRUSH)
#define ONPOWER_UP_SELECT       ONPOWER_UP_OFF
#else
#define ONPOWER_UP_SELECT       ONPOWER_UP_DEFAULT // ONPOWER_UP_STORE // 
#endif

enum ST_TRANS_TYPE{					// type of State transition
	ST_TRANS_LIGHTNESS  	= 0,	// share with power level
	#if (LIGHT_TYPE_CT_EN)
	ST_TRANS_CTL_TEMP,
	#endif
	#if (LIGHT_TYPE_HSL_EN)
	ST_TRANS_HSL_HUE,
	ST_TRANS_HSL_SAT,
	#endif
    #if (LIGHT_TYPE_SEL == LIGHT_TYPE_XYL)
	ST_TRANS_XYL_X,
	ST_TRANS_XYL_Y,
    #endif
    #if (LIGHT_TYPE_CT_EN)
	ST_TRANS_CTL_D_UV,              // no level model related, assign at the end should be better.
    #endif
	ST_TRANS_MAX,
    ST_TRANS_PUB_ONOFF = ST_TRANS_MAX,  // just use for publish,
};

enum{
	ST_PUB_TRANS_IDLE  	= 0,    // must 0
	ST_PUB_TRANS_ING,
	ST_PUB_TRANS_ALL_OK,        //
	ST_PUB_TRANS_MAX,
};

enum{
	OP_LC_ONOFF_TYPE_NONE   = 0,    // must 0
	OP_LC_ONOFF_TYPE_ON,
	OP_LC_ONOFF_TYPE_OFF,
};

typedef struct{
    u32 gpio;
    u8 id;
    u8 invert;
    u8 func;
    u8 rsv[1];
}light_res_hw_t;

typedef struct{
	u8 st[ST_TRANS_MAX + 1];    // + 1: for onoff publish flag
// following parameters is for inputting.	
	u8 no_dim_refresh_flag;     // input parameter
	u8 hsl_set_cmd_flag;        //
}st_pub_list_t;  // st_level_set_pub_list_t

typedef struct{
	u16 min;
	u16 max;
}light_range_u16_t;

typedef struct{
	s16 min;
	s16 max;
}light_range_s16_t;

typedef struct{
    s16 last;           // CT was save in level
	s16 def;			// default, all value transfer into s16, and CT not related to range
	s16 min;			// all value transfer into s16, and CT not related to range
	s16 max;			// all value transfer into s16, and CT not related to range
    u8 onoff;			// save for on power up
	u8 rsv;
}sw_level_save_t;

typedef struct{
	s32 step_1p32768;   // (1 / 32768 level unit)
	u32 remain_t_ms;	// unit ms: max 26bit: 38400*1000ms
	u16 delay_ms;		// unit ms
    s16 present;        // all value transfer into level, include CT.
    s16 present_1p32768;// (1 / 32768 level unit)
    s16 target;
}st_transition_t;

typedef struct{
	sw_level_save_t level[ST_TRANS_MAX];
#if (MD_LIGHT_CONTROL_EN && MD_SERVER_EN)
	u8 lc_onoff_target;
	u8 rsv_lc[3];
#endif
	// may add member here later
}light_res_sw_save_t;	// need save

typedef struct{
	st_transition_t trans[ST_TRANS_MAX];	// state transition
	u16 linear;
	u8 linear_set_flag;
	// may add member here later
}light_res_sw_trans_t;	// no need save

extern _align_4_ light_res_sw_save_t light_res_sw_save[LIGHT_CNT];
extern _align_4_ light_res_sw_trans_t light_res_sw[LIGHT_CNT];

#define set_level_current_type(idx, type)	// do{light_res_sw[idx].level_current_type = type;}while(0)
#define get_level_current_type(idx) 		//(light_res_sw[idx].level_current_type)
#define get_light_idx_from_level_md_idx(model_idx)  (model_idx / ELE_CNT_EVERY_LIGHT)
#define get_trans_type_from_level_md_idx(model_idx) (model_idx % ELE_CNT_EVERY_LIGHT)
#if LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define get_light_idx_from_onoff_md_idx(model_idx)  (model_idx / 2)
#define is_lc_model_from_onoff_md_idx(model_idx) 	(model_idx % 2)
#endif
//----------------
static inline u32 division_round(u32 val, u32 dividend)
{
	return (val + dividend/2)/dividend;
}

static inline u16 s16_to_u16(s16 val)
{
	return (val + 32768);
}

static inline s16 u16_to_s16(u16 val)
{
	return (val - 32768);
}

static inline u8 get_onoff_from_level(s16 level)
{
	return (level != LEVEL_OFF) ? 1 : 0;
}

static inline u16 get_lightness_from_level(s16 level)
{
	return (s16_to_u16(level));
}

static inline s16 get_level_from_lightness(u16 lightness)
{
	return (u16_to_s16(lightness));
}

static inline u16 get_ctl_temp_from_level(s16 level, u16 min, u16 max)
{
    if(max == min){
	    return max;
    }else{
        #if 0
        return min + ((level+32768)*(max-min))/65535;
        #else // use rounding, because CT value save in level,and should be restore to CT value accurately.
	    return min+division_round(((level+32768)*(max-min)),65535);
        #endif
	}
}

static inline s16 get_level_from_ctl_temp(u16 temp, u16 min, u16 max)
{
    // have been make sure temp is range in min and max before.
    if(max == min){
	    return 0x7fff;
	}else{
	    #if 0
	    return ((temp-min)*65535)/(max-min) - 32768;
	    #else // use rounding, because CT value save in level,and should be restore to CT value accurately.
	    return division_round((temp-min)*65535,(max-min))-32768;
        #endif
	}
}

static inline int get_level_set_st(int level_set_st1, int level_set_st2)
{
	return (level_set_st2 > level_set_st1 ? level_set_st2 : level_set_st1);
}

extern u8 light_pub_trans_step;
extern u8 *light_pub_model_priority;
extern u8 ct_flag;
void set_ct_mode(u8 mode);

void light_pwm_init();
void app_led_en (int id, int en);
void pwm_set_lum (int id, u16 y, int pol);
void mesh_global_var_init_light_sw();
s16 light_get_next_level(int idx, int st_trans_type);
s16 get_light_g_level_by_onoff(int idx, int on, int st_trans_type, int force_last);
void light_res_sw_g_level_last_set(int idx, int st_trans_type);
u16 light_ctl_temp_prensent_get(int light_idx);
u16 light_ctl_temp_target_get(int light_idx);

int light_g_level_set_idx(int idx, s16 level, int init_time_flag, int st_trans_type, st_pub_list_t *pub_list);
void get_light_pub_list(int st_trans_type, s16 present_level, s16 target_level, int pub_trans_flag, st_pub_list_t *pub_list);
void light_level_set_all(s16 level);
int light_onoff_idx(int idx, int on, int init_time_flag);
int light_onoff_idx_with_trans(u8 *set_trans, int idx);
void light_g_level_set_idx_with_trans(u8 *set_trans, int idx, int st_trans_type, int hsl_set_cmd_flag);
void light_res_sw_g_level_target_set(int idx, s16 level, int st_trans_type);
void light_onoff_all(u8 on);
int light_transition_proc();
void light_transition_proc_stop(int light_idx, int st_trans_type);
void light_par_save(int quick);
void light_par_save_proc();
void scene_status_change_check_all();
void set_on_power_up_onoff(int idx, int st_trans_type, u8 onoff);

int light_onoff_get(u8 *rsp, int idx);
int light_g_level_get(u8 *rsp, int idx, int st_trans_type);
s16 light_g_level_target_get(int idx, int st_trans_type);
s16 light_g_level_present_get(int idx, int st_trans_type);
u16 light_g_level_present_get_u16(int idx, int st_trans_type);
u8 light_g_onoff_present_get(int idx);
s16 light_g_level_last_get(int idx, int st_trans_type);
s16 light_g_level_def_get(int idx, int st_trans_type);
u16 light_g_level_def_get_u16(int idx, int st_trans_type);
int light_g_level_def_set(s16 val, int idx, int st_trans_type);
int light_g_level_def_set_u16(u16 val, int idx, int st_trans_type);
int light_g_level_range_get_u16(light_range_u16_t *p_range, int idx, int st_trans_type);
int light_g_level_range_get(light_range_s16_t *p_range, int idx, int st_trans_type);
int light_g_level_range_set(u16 min, u16 max, int idx, int st_trans_type);

s16 lum2level(u8 lum);
u8 level2lum(s16 level);
u16 lum2_lightness(u8 lum);
u8 lightness2_lum(u16 lightness);
u16 temp100_to_temp(u8 temp100);
u8 temp_to_temp100_hw(u16 temp);
u8 temp_to_temp100(u16 temp);

u8 light_lum_get(int idx, int target_flag);
u8 light_ct_lum_get(int idx, int target_flag);
void light_res_sw_load();
int set_light_linear_flag(int idx,u16 linear);
int clear_light_linear_flag(int idx);
u16 get_light_linear_val(int idx);
int is_linear_flag(int idx);
u8 light_remain_time_get(st_transition_t *p_trans);

// led
#define LGT_CMD_BLE_ADV					0xa0
#define LGT_CMD_BLE_CONN				0xa1

#define PROV_START_LED_CMD				0xc0
#define PROV_END_LED_CMD				0xc1

#define	LGT_CMD_SET_MESH_INFO           0xc5 // light event for get provision information OK, include mesh key, unicast address, etc. but not include key bind.
#define	LGT_CMD_SET_DEV_ADDR            0xc6 // 
#define	LGT_CMD_SET_SUBSCRIPTION        0xc7 // light event for set subscription address
#define	LGT_CMD_FRIEND_SHIP_OK          0xc8 // light event for friend ship establish ok.
#define	LGT_CMD_DUAL_MODE_MESH        	0xc9

#define LGT_CMD_SWITCH_POWERON 			0xd0
#define LGT_CMD_SWITCH_PROVISION		0xd1
#define LGT_CMD_SWITCH_CMD		        0xd2

#define LGT_CMD_PROV_SUC_EVE			LGT_CMD_SET_MESH_INFO

// default PWM function
#ifndef PWM_FUNC_R
#define PWM_FUNC_R  AS_PWM
#endif
#ifndef PWM_FUNC_G
#define PWM_FUNC_G  AS_PWM
#endif
#ifndef PWM_FUNC_B
#define PWM_FUNC_B  AS_PWM
#endif
#ifndef PWM_FUNC_W
#define PWM_FUNC_W  AS_PWM
#endif

void light_dim_refresh(int idx);
void light_dim_refresh_all();
void proc_led(void);
void cfg_led_event (u32 e);
int is_led_busy();
void cfg_led_event_stop ();
void led_onoff_gpio(u32 gpio, u8 on);
void rf_link_light_event_callback (u8 status);
void light_ev_with_sleep(u32 count, u32 half_cycle_us);
void show_factory_reset();
void reset_rx_onoff_cnt();
void increase_rx_onoff_cnt();
u16 get_rx_cnts();
void light_transition_onoff_manual(u8 onoff, u8 transit_t, u8 light_idx);
void set_keep_onoff_state_after_ota();
void clr_keep_onoff_state_after_ota();
int is_state_after_ota();

extern u16 g_op_access_layer_rx;

static inline u16 get_lightness_present(int light_idx)
{
	return (get_lightness_from_level(light_g_level_present_get(light_idx, ST_TRANS_LIGHTNESS)));
}

static inline u16 get_lightness_target(int light_idx)
{
	return (get_lightness_from_level(light_g_level_target_get(light_idx, ST_TRANS_LIGHTNESS)));
}

/**
  * @}
  */
    
/**
  * @}
  */

