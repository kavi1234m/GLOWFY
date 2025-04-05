/**
  **************************
  * @file    hal_sleep.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    07-12-2024
   * @brief   This file provides functions to manage the following
  *          breif the functionalities here:
  *           - function 1
  *           - function 2
  *           - function 3
  *
  *  @verbatim
  *
  *          ===================================================================
  *                             Working of chip/peripheral/Algorithm
  *          ===================================================================
  *
  *          Heading No.1
  *          =============
  *          Explanation
  *
  *          Heading No.2
  *          =============
  *          Explanation

  *          ===================================================================
  *                              How to use this driver / source
  *          ===================================================================
  *            -
  *            -
  *            -
  *            -
  *
  *  @endverbatim
  *
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2022 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */


/* Includes ------------------------------------------------------------------*/
#include "vendor/GLOWFY/HAL/hal_sleep.h"
#include "proj/mcu/compiler.h"
#include "drivers/8258/pm.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SLEEP_SET         1
#define ID_GET_COMMAND    2   //CODE FOR SCENE_REG_GET
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef void (*cbk_fun)();
cbk_fun lpn_callback;
u8 cpu_sts;
u8 send_request_flag=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/***********************************************************************************
  * @brief  initialization of mcu.
  * @param  none
  * @retval None
  ***********************************************************************************/
void hal_cpu_wakeup_init(void)
{
	cpu_wakeup_init();
}
/***********************************************************************************
  * @brief  selecting wakeup source as internal 32k RC.
  * @param  none
  * @retval None
  ***********************************************************************************/
void wakeup_source_for_sleep(void)
{
	blc_pm_select_internal_32k_crystal();

}
/***********************************************************************************
 * @brief      This function serves to determine whether mcu is waked up from deep retention.
 * @param[in]  none.
 * @return     1- yes , 0- no.
 ***********************************************************************************/
uint8_t is_mcu_wakeup_from_deep_retention(void)
{
	return pm_is_MCU_deepRetentionWakeup();
}

/***********************************************************************************
 * @brief       This function servers to set gpio wakeup source before sleep when key release.
 * @param[in]   e	- event code BLT_EV_FLAG_SUSPEND_ENTER
 * @param[in]   p	- NULL
 * @param[in]   n	- 0
 * @return      none
 * @note
 ***********************************************************************************/
void  hal_lpn_set_sleep_wakeup(u8 e, u8 *p, int n)
{
	LOG_USER_MSG_INFO(0, 0,"BEFORE SLEEP");
	if(!button_pin1_read() && !button_pin2_read())
	{
		bls_pm_setWakeupSource(PM_WAKEUP_PAD);
	}
}
/***********************************************************************************
  * @brief  init function from deep retention
  * @param  None
  * @retval None
  ***********************************************************************************/

_attribute_ram_code_ void ble_user_init_deepRetn(void)
{
    blc_app_loadCustomizedParameters();
	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (RF_POWER); // MY_RF_POWER_INDEX

	blc_ll_recoverDeepRetention();

	// should enable IRQ here, because it may use irq here, for example BLE connect.
	irq_enable();

#if LOW_POWER_MODE

	hal_user_init_peripheral(1);
    lpn_set_poll_ready(); // will call mesh_friend_ship_start_poll() in mesh_friend_ship_proc_LPN().
#endif
    #if ADC_ENABLE
	    hal_adc_init();
    #endif
}
/***********************************************************************************
  * @brief  function for put deepsleep without_retention and wakeup by pad
  * @param  None
  * @retval None
  ***********************************************************************************/
void set_deepsleep_without_retention_and_wakeup_by_pad(void)
{
     cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD,0);
}

/***********************************************************************************
  * @brief  function for put deepsleep with retentioin
  * @param  None
  * @retval None
  ***********************************************************************************/
void set_deepsleep_retention_mode(void)
{
	LOG_USER_MSG_INFO(0, 0,"DEEP SLEEP WITH RETENTION");
	cpu_long_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K,(PM_WAKEUP_PAD|PM_WAKEUP_TIMER),(POLL_TIMEOUT)*1000*32);

}
/***********************************************************
*	@brief     This function serves to set wakeup source
*	@param[in] void
*	@return	   none.
***********************************************************/
void button_wakeup_source_set(u8 e, u8 *p, int n)
{
	cpu_set_gpio_wakeup(GPIO_PD6, 0, 1);
    #if(NLC_TYPE == DIM_EN || NLC_TYPE == BSS_EN)
	cpu_set_gpio_wakeup(GPIO_PD5, 0, 1);
    #endif
}

/***********************************************************
  * @brief  function for to store the cpu status.
  * @param  None
  * @retval None
  ***********************************************************/
void store_cpu_status_for_lpn(uint8_t status)
{
	cpu_sts = status;
}
/***********************************************************
  * @brief  function for registering the lpn callback function.
  * @param  None
  * @retval None
  ***********************************************************/
void lpn_callback_register(void *(callback_fun))
{
	lpn_callback = callback_fun;
}
/***********************************************************
  * @brief  mesh friendship process for lpn
  * @param  1- bear //bear will have the value when the friendship reply comes from friendship device
  * @retval None
  ***********************************************************/
void mesh_friend_ship_proc_LPN(u8 *bear)
{
    if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){	// at GATT connected state, not friendship state.
        return ;
    }

    static u32 t_rec_delay_and_win = 0;
    u32 poll_retry_interval_ms = t_rec_delay_and_win;
	u32 timeout_ms = (poll_retry_interval_ms*1000) * (2*2+1)/2;    // confirm later

	if(!bear && is_mesh_adv_cmd_fifo_empty()){
	    if(fri_ship_proc_lpn.poll_retry && clock_time_exceed(fri_ship_proc_lpn.poll_tick, poll_retry_interval_ms*1000)){
	        fri_ship_proc_lpn.poll_retry--;
	        if(0 == fri_ship_proc_lpn.poll_retry){
	            if(FRI_ST_UPDATE == fri_ship_proc_lpn.status){
					mesh_friend_ship_retry();
	            }else{
	                friend_subsc_stop();
	                mesh_cmd_sig_lowpower_heartbeat();
	                friend_ship_disconnect_cb_lpn();
	                mesh_friend_ship_set_st_lpn(FRI_ST_REQUEST);    // restart establish procedure
	            }
	        }else{
	        	#if RETRY_POLL_AGAIN_FLIP_FSN_EN
	        	if((FRI_POLL_RETRY_MAX == fri_ship_proc_lpn.poll_retry) && is_in_mesh_friend_st_lpn()){
	        		mesh_lpn_par.poll.FSN = !mesh_lpn_par.poll.FSN;	// change to poll the 2nd round.
					LOG_MSG_LIB(TL_LOG_FRIEND, 0, 0,"LPN flip FSN, then try again");
	        	}
	        	#endif
	            friend_cmd_send_poll();  // retry
	        }
	    }
	    else if(subsc_list_retry.retry_cnt && clock_time_exceed(subsc_list_retry.tick, timeout_ms)){
	        subsc_list_retry.tick = clock_time() | 1;   // also refresh when send_subsc
	        subsc_list_retry.retry_cnt--;
	        if(0 == subsc_list_retry.retry_cnt){
	            mesh_friend_ship_set_st_lpn(FRI_ST_REQUEST);
	        }else{
	            LOG_MSG_LIB(TL_LOG_FRIEND, 0, 0,"friend_subsc_repeat_***********************");
	            friend_subsc_repeat();
	        }
	    }
	}

    mesh_cmd_bear_t *p_bear = (mesh_cmd_bear_t *)bear;
    //mesh_cmd_nw_t *p_nw = &p_bear->nw;
    mesh_cmd_lt_ctl_unseg_t *p_lt_ctl_unseg = &p_bear->lt_ctl_unseg;
    u8 op = -1;
    if(bear){
	    op = p_lt_ctl_unseg->opcode;
    }

    if(0 == fri_ship_proc_lpn.status){ // means at friendship establish ok state or before state of send request message.
        if(bear){
            if(CMD_CTL_SUBS_LIST_CONF == op){
                mesh_ctl_fri_subsc_list_t *p_subsc = CONTAINER_OF(p_lt_ctl_unseg->data,mesh_ctl_fri_subsc_list_t,TransNo);
                if(p_subsc->TransNo == (subsc_list_retry.TransNo)){   //TransNo have increased
                	LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)&p_subsc->TransNo, sizeof(p_subsc->TransNo),"rcv sub list confirm:");
                    if(SUBSC_ADD == subsc_list_retry.retry_type){
                    	   sub_list_add();
//                        friend_subsc_list_add_adr((lpn_adr_list_t *)(&lpn_subsc_list.adr), (lpn_adr_list_t *)(&subsc_list_retry.adr), subsc_list_retry.subsc_cnt);
                    }
                    friend_subsc_stop();
                }
            }else if(CMD_CTL_UPDATE == op){
                mesh_ctl_fri_update_t *p_update = (mesh_ctl_fri_update_t *)(p_lt_ctl_unseg->data);
				LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_update, sizeof(mesh_ctl_fri_update_t),"NW_IVI %d, sno: 0x%02x%02x%02x, rcv friend update:", p_bear->nw.ivi, p_bear->nw.sno[2], p_bear->nw.sno[1], p_bear->nw.sno[0]);
                iv_update_key_refresh_rx_handle(&p_update->flag, p_update->IVIndex);
                if(cpu_sts != 2)
                {
                	lpn_callback(SLEEP_SET);
                	//cpu_long_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K,(PM_WAKEUP_PAD|PM_WAKEUP_TIMER),(1*60)*1000*32);
                }
            }
        }else{
            if(is_friend_ship_link_ok_lpn() && is_mesh_adv_cmd_fifo_empty()  && clock_time_exceed_ms(fri_ship_proc_lpn.poll_time_ms,get_lpn_poll_interval_ms())){ // send poll after mesh message and subscription list add done.
            	if(!subsc_list_retry.tick){
					if(mesh_lpn_subsc_pending.op){
						mesh_lpn_sleep_prepare(CMD_ST_POLL_MD);
					}
					else{
						mesh_friend_ship_start_poll();
					}
				}
			}
        }
    }else{
        switch(fri_ship_proc_lpn.status){   // Be true only during establishing friend ship.
            case FRI_ST_REQUEST:
				if(is_mesh_adv_cmd_fifo_empty() && clock_time_exceed(fri_ship_proc_lpn.req_tick, FRI_REQ_TIMEOUT_MS * 1000)){
	                fri_ship_proc_lpn.req_tick = clock_time();
                    if(send_request_flag < LPN_REQ_SEND_COMMAND_COUNT)
                    {
                    	friend_cmd_send_request();
                    	send_request_flag++;
                    }
                    else
                    {
                    	send_request_flag=0;
                    	lpn_callback(SLEEP_SET);
                    	//cpu_long_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K,(PM_WAKEUP_PAD|PM_WAKEUP_TIMER),2764800000);//24*60*60*1000*32-24 hours
                    }

	                // init par
	                mesh_friend_ship_clear_LPN();
	                mesh_lpn_par.LPNAdr = ele_adr_primary;
	                mesh_friend_ship_set_st_lpn(FRI_ST_OFFER);
				}
                break;
            case FRI_ST_OFFER:
                if(bear){
                    if(CMD_CTL_OFFER == p_lt_ctl_unseg->opcode){
                        if(0 != lpn_rx_offer_handle(bear)){
                            break;
                        }
                    }
                }else{
                    if(clock_time_exceed(fri_ship_proc_lpn.req_tick, FRI_ESTABLISH_PERIOD_MS*1000)){
                        if(mesh_lpn_par.FriAdr){
                            mesh_lpn_par.link_ok = 1;
                            mesh_friend_key_update_all_nk(0, 0);
                        }
                        mesh_friend_ship_set_st_lpn(FRI_ST_POLL);
                    }
                }
                break;
            case FRI_ST_POLL:
                if(is_friend_ship_link_ok_lpn()){
					if(is_mesh_adv_cmd_fifo_empty()){
	                    mesh_lpn_par.poll.FSN = 0;   // init
	                    // send poll
	                    mesh_friend_ship_start_poll();
	                    t_rec_delay_and_win = mesh_lpn_par.req.RecDelay + mesh_lpn_par.offer.RecWin;
	                    mesh_friend_ship_set_st_lpn(FRI_ST_UPDATE);
					}
                }else{
                    lpn_no_offer_handle();
                }
                break;
            case FRI_ST_UPDATE:
                if(bear){   // current state is establishing friend ship
                    if(CMD_CTL_UPDATE == p_lt_ctl_unseg->opcode){
                        mesh_ctl_fri_update_t *p_update = (mesh_ctl_fri_update_t *)(p_lt_ctl_unseg->data);
						LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_update, sizeof(mesh_ctl_fri_update_t),"rcv friend update:");
                        memcpy(&mesh_lpn_par.update, p_update, sizeof(mesh_ctl_fri_update_t));
                        //friend ship establish done
                        mesh_lpn_par.req.PreAdr = mesh_lpn_par.FriAdr;
                        #if !PTS_TEST_EN
						mesh_cmd_sig_lowpower_heartbeat();
						#endif
                        iv_update_key_refresh_rx_handle(&p_update->flag, p_update->IVIndex);
                        mesh_friend_ship_proc_init_lpn();

                        friend_ship_establish_ok_cb_lpn();

                        if(cpu_sts != 2)
                        {
                    #if NLC_TYPE == BSS_EN
                        	if(check_publish_address(SIG_MD_SCENE_C,0) == 0)
                        	{
                        		lpn_callback(SLEEP_SET);
                        	}
                        	else
                        	{
                        		lpn_callback(ID_GET_COMMAND);
                        	}
                    #else
                        	lpn_callback(SLEEP_SET);
                    #endif
                        }
                    }
                }else{
                	#if 0 // timeout checking in count down of poll retry. if not, sometimes will trigger retry request when poll retry is not 0.
                    if(clock_time_exceed(fri_ship_proc_lpn.poll_tick, t_rec_delay_and_win*1000)){
                    	mesh_friend_ship_retry();
                    }
                    #endif
                }
                break;
            default:
                break;
        }
    }
}
 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/
