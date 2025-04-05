/**
  **************************
  * @file    application.c
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
#include "application.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/***button event***********/
#define BUTTON_SINGLE_PRESS                                  0x01
#define BUTTON_LONG_PRESS                                    0x02
#define BEFORE_PROVISION_SLEEP                               0x03
/***ble event ************/
#define ADVERTISING                                          0x01
#define PROVISION_SUCCESS                                    0x02
#define PROVISION_FAILED                                     0X03
#define FRIENDSHIP_ESTABLISH_OK                              0X04

/******dimmer control event*********/
#define BRIGHTNESS_INCREASE  1
#define BRIGHTNESS_DECREASE  2

/*********cpu wakeup source*************/

#define POWER_UP              0x00
#define WATCHDOG_RESET        0x01
#define BUTTON_WAKEUP         0x02
#define FACTORY_RESET         0x03
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 cpu_status;
u8 provision_status;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/********************************************************************************
  * @brief  button press event
  * @param  event id
             1-single press
             2-long press
  * @retval None
  ********************************************************************************/
void button_press_event(uint8_t event_id)
{
	switch(event_id)
	{
	    case BUTTON_SINGLE_PRESS:
	    	send_onoff_command();
	    	break;
	    case  BUTTON_LONG_PRESS :
	    	node_reset();
	    	break;
	    case BEFORE_PROVISION_SLEEP:
	    	set_deepsleep_without_retention_and_wakeup_by_pad();
	    	break;

	    default :
	    	break;

	}

}
void dimmer_control_event(uint8_t event_id)
{
      switch(event_id)
      {
      case BRIGHTNESS_INCREASE:
    	     brightness_control_using_dimmer(BRIGHTNESS_INCREASE);
    	     break;
      case BRIGHTNESS_DECREASE:
    	  brightness_control_using_dimmer(BRIGHTNESS_DECREASE);
    	  break;
      }
}
/********************************************************************************
  * @brief  ble_mesh_event
  * @param  event id
             ADVERTISING       -advertising
             PROVISION_SUCCESS -deep sleep with retention
             PROVISION_FAILED  -deep sleep without retention
             FRIENDSHIP_ESTABLISH_OK - friendship established successfully
  * @retval None
  ********************************************************************************/
void ble_mesh_event(uint8_t event_id)
{
	switch(event_id)
	{
	    case ADVERTISING       :
	    	led_1_sec_on_2_sec_off_30_sec();
	        break;

	    case PROVISION_SUCCESS :
	    	set_deepsleep_retention_mode();
	    	break;

	    case PROVISION_FAILED  :
	    	set_deepsleep_without_retention_and_wakeup_by_pad();
	    	break;

	    default :
	    	break;

	}

}
/********************************************************************************
  * @brief  lpn_event
  * @param  None
  * @retval None
  ********************************************************************************/
void lpn_event(void)
{
	set_deepsleep_retention_mode();
}
/********************************************************************************
  * @brief  ble data processing loop
  * @param  None
  * @retval None
  ********************************************************************************/
void ble_data_processing(void)
{
    ble_entry();
    sig_mesh_start();
	button_event_check();
    led_event();
	lpn_state_check();

}
/********************************************************************************
  * @brief  appMain_init function
  * @param  None
  * @retval None
  ********************************************************************************/
void appMain_init(void)
{
	button_press_callback_register(button_press_event);
    dimmer_control_callback_register(dimmer_control_event);
	lpn_callback_register(lpn_event);
    ble_mesh_callback_register(ble_mesh_event);

    cpu_status =get_cpu_wakeup_source();
    LOG_USER_MSG_INFO(0, 0,"Wake Up :%u",cpu_status);
    provision_status =check_provision_state();
	if( is_mcu_wakeup_from_deep_retention())
	{
		ble_user_init_deepRetn ();
	}
	else
	{
		ble_init();
		if(cpu_status != FACTORY_RESET && !provision_status)
		{
			ble_adv_enable_disable(0);
		}
	}
     while(1)
     {
	   	 hal_watchdog_clear();
	   	 ble_data_processing();
     }
//	blc_smp_setSecurityLevel
}

 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/
