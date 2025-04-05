/**
  **************************
  * @file    hal_ble_mesh.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    10-Dec-2024
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
//#include "stack/ble/ll/ll_conn/ll_conn_csa.h"
#include "tl_common.h"
#include "vendor/GLOWFY/HAL/hal_ble_mesh.h"
#include "proj/mcu/compiler.h"
/* Private typedef -----------------------------------------------------------*/
MYFIFO_INIT(blt_rxfifo, 64, 8);
MYFIFO_INIT(blt_txfifo, 40, 8);

typedef void (*cbk_fun)();
cbk_fun ble_mesh_callback;
/* Private define ------------------------------------------------------------*/
#define ADVERTISING                   0x01
#define PROVISION_SUCCESS             0x02
#define PROVISION_FAILED              0X03
#define FRIENDSHIP_ESTABLISH_OK       0X04
#define BATTERY_STATUS                0x05

/*********cpu wakeup source*************/

#define POWER_UP              0
#define WATCHDOG_RESET        1
#define BUTTON_WAKEUP         2
#define FACTORY_RESET         3

/***********dimmer*************/
#define DIMMER_BRIGHTNESS_INCREASE     21
#define DIMMER_BRIGHTNESS_DECREASE     22

/*********** scene ***********/
#define SCENE_RECALL_WITH_ID_INCREASE  31
#define SCENE_RECALL_WITH_ID_DECREASE  32
#define SCENE_ID_GET                   33

#define SCENE_SAMPLE_ID            0X0001

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 lpn_provision_ok;

//u8 node_binding_tick_=0;
u8 lpn_mode_ = LPN_MODE_NORMAL;
u32 lpn_mode_tick_ = 0;
u32 lpn_wakeup_tick_ = 0;
u8 cmd_success_flag=0;
u32 wakeup_time;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/***********************************************************
  * @brief  function for registering the sleep callback function.
  * @param  None
  * @retval None
  ***********************************************************/
void ble_mesh_callback_register(void *(callback_fun))
{
	ble_mesh_callback = callback_fun;
}
/***********************************************************
 * @brief       This function server to manage LPN work state.
 * @param       None
 * @return      none
 ***********************************************************/
void lpn_state_check(uint8_t cpu_status)
{
    if((cpu_status != POWER_UP)||(cpu_status == POWER_UP && check_provision_state()))
    {
	static u8 fri_request_send = 1;
	if(fri_request_send && (LPN_MODE_GATT_OTA != lpn_mode_)){
		fri_request_send = 0;
		if(cpu_status == FACTORY_RESET)
		{
			if(!check_provision_state())
			{
				ble_mesh_callback(ADVERTISING);
	        }
			else
			{
				led_onoff_gpio(GPIO_LED,0);
			}

		}

	}
	if(lpn_provision_ok){
		if((blc_ll_getCurrentState() == BLS_LINK_STATE_ADV))
		{
			if(!is_friend_ship_link_ok_lpn() && (FRI_ST_IDLE == fri_ship_proc_lpn.status))
			{
				mesh_friend_ship_set_st_lpn(FRI_ST_REQUEST);// prevent abnormal working time. if no sleep mode, lpn_wakeup_tick need to be refreshed which can refer to demo code in mesh_lpn_pm_proc().
		     }
			     if( cmd_success_flag )
			     {
//                      if(cmd_success_flag == 1)
//                      {
//                    	  ble_mesh_callback(BATTERY_STATUS);
//                      }
//                      cmd_success_flag = 2;
				      if(clock_time_exceed(wakeup_time,1*1000*1000))
				      {
					      cmd_success_flag=0;
						  ble_mesh_callback(PROVISION_SUCCESS);
			       	  }
			      }

        }
	}else{
	    if(!is_provision_success()){
    	        if(clock_time_exceed(0,(PROVISION_TIME)*1000*1000)){
    	        	ble_mesh_callback(PROVISION_FAILED);
    	        }
	    }else{
    	    if((!lpn_provision_ok)&& node_binding_tick && clock_time_exceed(node_binding_tick,LPN_START_REQUEST_AFTER_BIND_MS*1000))
	    	{
				lpn_provision_ok = 1;// provison and key bind finish
				cmd_success_flag=0;
				wakeup_time = clock_time();
				mesh_friend_ship_set_st_lpn(FRI_ST_REQUEST);
				if(BLS_LINK_STATE_CONN == blc_ll_getCurrentState()){
					bls_ll_terminateConnection(0x13); // disconnect to establish friendship
				}
    	    }
	    }
	    return ;
	 }
    }
	return;
}
/***********************************************************
*	@brief     This function to start advertise
*	@param[in] none.
*	@return	   none.
***********************************************************/
void ble_adv_start(void)
{
	bls_ll_setAdvParam(ADV_INTERVAL_MIN, ADV_INTERVAL_MAX, \
			 	 	 	 	 	     ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
			 	 	 	 	 	     0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

	// normally use this settings
	bls_ll_setAdvEnable(1);  //adv enable

}
/*********************************
  * @brief  put into sleep mode after there is no event
  * @param  None
  * @retval None
  ********************************/
void sleep_set(void)
{
	wakeup_time = clock_time();
    cmd_success_flag =1;
}
/*********************************
  * @brief  function to check publish address
  * @param  None
  * @retval None
  ********************************/
uint16_t check_publish_address(u32 model_id,u8 element_offset)
{
    u16 pub_addr=0;
    u8 ele_offset=element_offset;
    model_common_t *p_model;
    u8 model_idx = 0;
    p_model = (model_common_t *)mesh_find_ele_resource_in_model(ele_adr_primary + ele_offset, model_id, 1, &model_idx, 0);
	if(p_model && p_model->pub_adr)
	{
		pub_addr = p_model->pub_adr;
	}
	if(pub_addr)
	{
		return pub_addr;
	}
	return 0;
}
/*********************************
  * @brief  function to send generic on/off command
  * @param  None
  * @retval None
  ********************************/
void send_onoff_command(void)
{
	static u32 test_onoff=0;
	wakeup_time = clock_time();
    u16 pub_addr=0;
    u8 ele_offset=0;
	pub_addr = check_publish_address(SIG_MD_G_ONOFF_C,ele_offset);
	if(pub_addr)
	{
    	access_cmd_onoff(pub_addr, 0, (test_onoff++) & 1, CMD_NO_ACK, 0);
	}

}
/*********************************
  * @brief  function for brightness increase using dimmer
  * @param  type - increase/decrease
  * @retval None
  ********************************/
void brightness_control_using_dimmer(uint8_t type)
{
	transition_par_t transition;
	transition.steps = 5;   // 1 step
	transition.res = TRANSITION_STEP_RES_100MS;
	transition.delay = 0;
	s16 delta_level = (65536/10);  //(0, 3641, 7282, 10922, 14563, 18204, 21845, 25485, 29126)
    u16 pub_addr = 0;
    u8 ele_offset=0;
	pub_addr = check_publish_address(SIG_MD_G_LEVEL_C,ele_offset);
	if(pub_addr)
	{

		if(type == DIMMER_BRIGHTNESS_INCREASE)
		{
			LOG_USER_MSG_INFO(0, 0,"DIMMER_BRIGHTNESS_INCREASE");
			access_cmd_set_level_move(pub_addr,0,delta_level,CMD_NO_ACK,&transition);

		}
		else if(type == DIMMER_BRIGHTNESS_DECREASE)
		{
			LOG_USER_MSG_INFO(0, 0,"DIMMER_BRIGHTNESS_DECREASE");
			access_cmd_set_level_move(pub_addr,0,-(delta_level),CMD_NO_ACK,&transition);


		}
		else
		{
			LOG_USER_MSG_INFO(0, 0,"STOP");
			access_cmd_set_level_move(pub_addr,0,0,CMD_NO_ACK,&transition);
		}
	}
}

/*********************************
  * @brief  function for scene_store_and_scene_recall
  * @param  type - scene store/scene recall
  * @retval None
  ********************************/
void scene_store_and_scene_recall(uint8_t type,uint16_t scene_id)
{
    u16 pub_addr=0;
    u8 ele_offset=0;
	pub_addr = check_publish_address(SIG_MD_SCENE_C,ele_offset);
	if(pub_addr)
	{
#if NLC_TYPE == BSS_EN
        if(type == SCENE_RECALL_WITH_ID_INCREASE)
        {
        	access_cmd_scene_recall(pub_addr, 0,scene_id,CMD_NO_ACK,NULL);
        }
        else if(type == SCENE_RECALL_WITH_ID_DECREASE)
        {
        	access_cmd_scene_recall(pub_addr, 0,scene_id,CMD_NO_ACK,NULL);
        }
        else
        {
        	access_cmd_scene_reg_get(pub_addr,0);
          #if LOW_POWER_MODE
        	lpn_set_poll_ready();
          #endif
        }
#endif

	}
}
/*********************************
  * @brief  Soft Timer init
  * @param  None
  * @retval None
  ********************************/
void soft_timer_init(void)
{
#if LOW_POWER_MODE
    soft_timer_mesh_adv_proc();
#endif
    blt_soft_timer_process(MAINLOOP_ENTRY);
}
/*********************************
  * @brief  ble entry
  * @param  None
  * @retval None
  ********************************/
void ble_entry(void)
{
    soft_timer_init();
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop ();

}
/***********************************************************
* @brief  handle BLE event function.
* @param  None
* @retval None
***********************************************************/
int app_event_handler (u32 h, u8 *p, int n)
{
	static u32 event_cb_num;
	event_cb_num++;
	int send_to_hci = 1;

	if (h == (HCI_FLAG_EVENT_BT_STD | HCI_EVT_LE_META))		//LE event
	{
		u8 subcode = p[0];

	//------------ ADV packet --------------------------------------------
		if (subcode == HCI_SUB_EVT_LE_ADVERTISING_REPORT)	// ADV packet
		{
			event_adv_report_t *pa = (event_adv_report_t *)p;
			if(LL_TYPE_ADV_NONCONN_IND != (pa->event_type & 0x0F)){
				return 0;
			}
			send_to_hci = (0 == app_event_handler_adv(pa->data, MESH_BEAR_ADV, 1));

		}

	//------------ connection complete -------------------------------------
		else if (subcode == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
		{
			event_connection_complete_t *pc = (event_connection_complete_t *)p;
			if (!pc->status)							// status OK
			{
				//app_led_en (pc->handle, 1);

				//peer_type = pc->peer_adr_type;
				//memcpy (peer_mac, pc->mac, 6);
			}

			proxy_cfg_list_init_upon_connection(BLS_CONN_HANDLE);
			mesh_service_change_report(BLS_CONN_HANDLE);
		}

	//------------ connection update complete -------------------------------
		else if (subcode == HCI_SUB_EVT_LE_CONNECTION_UPDATE_COMPLETE)	// connection update
		{
		}
	}

	//------------ disconnect -------------------------------------
	else if (h == (HCI_FLAG_EVENT_BT_STD | HCI_EVT_DISCONNECTION_COMPLETE))		//disconnect
	{

		event_disconnection_t	*pd = (event_disconnection_t *)p;
		//app_led_en (pd->handle, 0);
		//terminate reason
		if(pd->reason == HCI_ERR_CONN_TIMEOUT){

		}
		else if(pd->reason == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

		}
		else if(pd->reason == SLAVE_TERMINATE_CONN_ACKED || pd->reason == SLAVE_TERMINATE_CONN_TIMEOUT){

		}

		mesh_ble_disconnect_cb(p);
	}

	if (send_to_hci)
	{
		//blc_hci_send_data (h, p, n);
	}

	return 0;
}
/***********************************************************
* @brief  ble_init function.
* @param  None
* @retval None
***********************************************************/
void ble_init(void)
{
//		app_battery_power_check_and_sleep_handle(0); //battery check must do before OTA relative operation
		mesh_global_var_init();

		app_flash_protection_operation(FLASH_OP_EVT_APP_INITIALIZATION, 0, 0);
		blc_appRegisterStackFlashOperationCallback(app_flash_protection_operation); //register flash operation callback for stack

		set_blc_hci_flag_fun(0);// disable the hci part of for the lib .
#if LOW_POWER_MODE
		lpn_provision_ok = is_net_key_save();
#endif
		proc_telink_mesh_to_sig_mesh();		// must at first

		blc_app_loadCustomizedParameters();  //load customized freq_offset cap value and tp value


		////////////////// BLE stack initialization ////////////////////////////////////
		ble_mac_init();


		blc_ll_initBasicMCU();                      //mandatory
		blc_ll_initStandby_module(tbl_mac);				//mandatory

		blc_ll_initAdvertising_module(tbl_mac); 	//adv module: 		 mandatory for BLE slave,
		blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,

#if LOW_POWER_MODE
		blc_ll_initPowerManagement_module();		//pm module:		 optional
//		ENABLE_SUSPEND_MASK;

		blc_pm_setDeepsleepRetentionThreshold(50, 30); // threshold to enter retention
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(400); // retention early wakeup time
		bls_pm_registerFuncBeforeSuspend(app_func_before_suspend);
		bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &hal_lpn_set_sleep_wakeup);
//		bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &lpn_process_key);
#else
	    bls_pm_setSuspendMask (SUSPEND_DISABLE);  //PM_DEEPSLEEP_RETENTION_ENABLE
#endif
		blc_l2cap_register_handler (app_l2cap_packet_receive);
		///////////////////// USER application initialization ///////////////////

		u8 status = bls_ll_setAdvParam( ADV_INTERVAL_MIN, ADV_INTERVAL_MAX, \
			                        	ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
				 	 	 	 	 	     0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

		if(status != BLE_SUCCESS){  //adv setting err
			write_reg8(0x8000, 0x11);  //debug
			while(1);
		}

		// normally use this settings
		blc_ll_setAdvCustomedChannel (37, 38, 39);
		bls_ll_setAdvEnable(1);  //adv enable

		rf_set_power_level_index (RF_POWER);
	    blc_hci_le_setEventMask_cmd(HCI_LE_EVT_MASK_ADVERTISING_REPORT|HCI_LE_EVT_MASK_CONNECTION_COMPLETE);


		rf_pa_init();
		bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, (blt_event_callback_t)&mesh_ble_connect_cb);
		blc_hci_registerControllerEventHandler(app_event_handler);		//register event callback
		bls_set_advertise_prepare (app_advertise_prepare_handler);
//		bls_ota_registerStartCmdCb(entry_ota_mode);
//		bls_ota_registerResultIndicateCb(show_ota_result);

#if LOW_POWER_MODE
		if(!lpn_provision_ok)
		{
			app_enable_scan_all_device ();
		}
#endif

		// mesh_mode and layer init
		mesh_init_all();


		{bls_ota_clearNewFwDataArea(0);	 //must
		}
		blc_gap_peripheral_init();    //gap initialization

		mesh_scan_rsp_init();
		my_att_init (provision_mag.gatt_mode);
		blc_att_setServerDataPendingTime_upon_ClientCmd(10);
		system_time_init();
    #if LOW_POWER_MODE
	    blt_soft_timer_init();
		hal_user_init_peripheral(0);
		mesh_lpn_gatt_adv_refresh();
	#endif
}

/***********************************************************
*	@brief     This function serves to initiate the mode of RF
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
***********************************************************/
void hal_rf_drv_init(void)
{
    rf_drv_init(RF_MODE_BLE_1M);

}
/***********************************************************
*	@brief     This function serves to ble_adv_enable_disable
*	@param[in] enable or disable
*	@return	   none.
***********************************************************/
void ble_adv_enable_disable(uint8_t level)
{
	bls_ll_setAdvEnable(level);
}
/***********************************************************
*	@brief     This function serves to lpn advertisement ineterval
*	@param[in] void
*	@return	   none.
***********************************************************/
uint32_t hal_mesh_lpn_adv_interval_update(u8 adv_tick_refresh)
{
	int ret = 0;
	u32 interval;
	interval =GET_ADV_INTERVAL_MS(ADV_INTERVAL_UNIT);
	ret = bls_ll_setAdvParam_interval(interval, 0);

	if(adv_tick_refresh){
		extern u32 blt_advExpectTime;
		blt_advExpectTime = clock_time() + blta.adv_interval;
	}

	return ret;
}
/***********************************************************
*	@brief     This function to get poll interval
*	@param[in] void
*	@return	   poll interval time.
***********************************************************/
u32 hal_get_lpn_poll_interval_ms(void)
{

	return FRI_POLL_INTERVAL_MS;
}
/***********************************************************
*	@brief     This function to check friendship establishment
*	@param[in] void
*	@return	   1-success 0- failed.
***********************************************************/
uint8_t is_friend_ship_link_ok_lpn_(void)
{
	return mesh_lpn_par.link_ok;
}
/***********************************************************
*	@brief     This function to check provision state
*	@param[in] void
*	@return	   1-success 0- failed.
***********************************************************/
uint8_t check_provision_state(void)
{
	return is_net_key_save();
}
/***********************************************************
*	@brief     This function for start mesh operation
*	@param[in] void
*	@return	   none.
***********************************************************/
void sig_mesh_start(void)
{
    CB_USER_MAIN_LOOP();
	// node identity proc
	//BSSMP/BSS/FEAT/BV-01-I,IUT needs to send NODE_IDENTITY_TYPE broadcast packet for proxy connection
	mesh_switch_identity_proc();
    // provision loop proc
	mesh_prov_proc_loop();
	// mesh beacon proc
	mesh_beacon_send_proc();
	// mesh proxy proc
	mesh_proxy_sar_timeout_terminate();
	// node reset
	proc_node_reset();
	#if (!WIN32)
	mesh_ota_reboot_proc();
	#endif
    // publish proc
	mesh_pub_period_proc();
	// mesh proc
	mesh_seg_ack_poll();
#if LOW_POWER_MODE
	if(is_lpn_support_and_en){
	    #if FEATURE_LOWPOWER_EN
		mesh_friend_ship_proc_LPN(NULL);
        mesh_main_loop_LPN();
		#endif
	}else if(is_fn_support_and_en){
		mesh_friend_ship_proc_FN(NULL);
	}
#endif
	    #if MESH_BLE_NOTIFY_FIFO_EN
	mesh_notifyfifo_rxfifo();
	    #endif
	mesh_tid_timeout_check();
	mesh_flash_save_check();
	system_time_run(); // must at last of main_loop(), before mesh_lpn_proc_suspend()
}
 /***********End of function_demo() function****************/
/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

