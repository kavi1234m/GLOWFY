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
#include "vendor/GLOWFY/APPLICATION/application.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/************event type*************/
#define PRESS                          1
#define DIMMER                         2
#define SCENE                          3
/***button event***********/
#define BUTTON_SINGLE_PRESS            11
#define BUTTON_LONG_PRESS              12
#define BEFORE_PROVISION_SLEEP         13
/******dimmer control event*********/
#define BRIGHTNESS_INCREASE            21
#define BRIGHTNESS_DECREASE            22
#define BRIGHTNESS_STOP                23

/*********** scene ***********/
#define SCENE_RECALL_WITH_ID_INCREASE  31
#define SCENE_RECALL_WITH_ID_DECREASE  32
#define SCENE_ID_GET                   33

/***ble event ************/
#define ADVERTISING                     1
#define PROVISION_SUCCESS               2
#define PROVISION_FAILED                3
#define FRIENDSHIP_ESTABLISH_OK         4
#define BATTERY_STATUS                  5

/***********lpn event**********/
#define SLEEP_SET                      1
#define ID_GET_COMMAND                 2

/*********cpu wakeup source*************/

#define POWER_UP              0x00
#define WATCHDOG_RESET        0x01
#define BUTTON_WAKEUP         0x02
#define FACTORY_RESET         0x03
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 cpu_status;
u8 provision_status;

// scene
uint16_t scene_id[16];
uint8_t no_of_scene_id;
s8 id_num =-1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/********************************************************************************
  * @brief  button press event
  * @param  event id
             1-single press
             2-long press
  * @retval None
  ********************************************************************************/

void button_press_event(uint8_t event_type,uint8_t event_id)
{
      if(event_type == PRESS)
      {
    	  switch(event_id)
    	  {
             case BUTTON_SINGLE_PRESS:
                  #if (NLC_TYPE == OCS_EN || NLC_TYPE == OCS_EN_AND_ALS_EN)
 	                   send_sensor_status();
                  #else
 	                   send_onoff_command();
                  #endif
 	               break;
             case  BUTTON_LONG_PRESS :
 	               node_reset();
 	               break;
             case  BEFORE_PROVISION_SLEEP:
 	               set_deepsleep_without_retention_and_wakeup_by_pad();
 	               break;
    	  }
      }
      else if(event_type == DIMMER)
      {
    	 switch(event_id)
    	 {
              case BRIGHTNESS_INCREASE:
  	               brightness_control_using_dimmer(BRIGHTNESS_INCREASE);
  	               break;
              case BRIGHTNESS_DECREASE:
  	               brightness_control_using_dimmer(BRIGHTNESS_DECREASE);
  	               break;
              case BRIGHTNESS_STOP:
  	               brightness_control_using_dimmer(BRIGHTNESS_STOP);
  	               break;
    	 }
      }
      else if(event_type == SCENE)
      {
    	  switch(event_id)
    	  {
               case SCENE_RECALL_WITH_ID_INCREASE:
            	   if(id_num == -1)
            	   {
            		   id_num =0;
            	   }
            	   else
            	   {
               	      id_num =id_num+1;
               	      if(id_num > (no_of_scene_id-1))
               	      {
               	    	  id_num = 0;
               	      }
            	   }
         	        scene_store_and_scene_recall(SCENE_RECALL_WITH_ID_INCREASE,scene_id[id_num]);
         	        break;
               case SCENE_RECALL_WITH_ID_DECREASE:
            	    if(id_num == -1)
            	    {
            	    	id_num = 0;
            	    }
            	    else
            	    {
                   	    id_num = id_num-1;
                   	    if(id_num < 0)
                     	{
                   		   id_num =no_of_scene_id - 1;
                    	}
            	    }
         	        scene_store_and_scene_recall(SCENE_RECALL_WITH_ID_DECREASE,scene_id[id_num]);
         	        break;
               case SCENE_ID_GET     :
            	    no_of_scene_id = 0;
         	        scene_store_and_scene_recall(SCENE_ID_GET,0);
         	        break;
    	  }
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
       #if LOW_POWER_MODE
//	    	fetch_battery_voltage_and_send_battery_status();
       #endif
	    	set_deepsleep_retention_mode();
	    	break;

	    case PROVISION_FAILED  :
	    	set_deepsleep_without_retention_and_wakeup_by_pad();
	    	break;

	    case BATTERY_STATUS    :
#if LOW_POWER_MODE
	    	 fetch_battery_voltage_and_send_battery_status();
#endif
	    	 break;

	    default :
	    	break;

	}

}
/********************************************************************************
  * @brief  is_scene_stored
  * @param  1-scene id
  * @retval 1- if scene id already stored
  *         0- if scene id hasn't stored
  ********************************************************************************/
uint8_t is_scene_stored(uint16_t scene_id_)
{
    for (uint8_t i = 0; i < no_of_scene_id; i++){

        if (scene_id[i] == scene_id_){
            return 1;  // Scene already stored
        }
    }
    return 0;
}
/********************************************************************************
  * @brief  store scene id
  * @param  None
  * @retval None
  ********************************************************************************/
void store_scene_id(uint8_t *buffer,uint8_t buf_len)
{
		uint8_t scene_id_count = (buf_len - 3)/2;
		uint16_t scene_id_num = 0;
		for(uint8_t i=0,j=0;i< scene_id_count;i++,j=j+2){

			scene_id_num =  buffer[j+3]|(buffer[j+4] << 8);
			if(is_scene_stored(scene_id_num) == 0){
				scene_id[no_of_scene_id++] = scene_id_num;
			}
			scene_id_num = 0;
		}
}
/********************************************************************************
  * @brief  lpn_event
  * @param  None
  * @retval None
  ********************************************************************************/
void lpn_event(uint8_t event_id)
{
	switch (event_id){
	    case SLEEP_SET:
             #if LOW_POWER_MODE
//	            fetch_battery_voltage_and_send_battery_status();
             #endif
	            set_deepsleep_retention_mode();
	            break;
     #if NLC_TYPE == BSS_EN
	    case ID_GET_COMMAND:
	    	  button_press_event(SCENE,SCENE_ID_GET);
	    	  break;
     #endif

	}
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
#if LOW_POWER_MODE
    lpn_state_check(cpu_status);
#endif

}
/********************************************************************************
  * @brief  appMain_init function
  * @param  None
  * @retval None
  ********************************************************************************/
void appMain_init(void)
{
    cpu_status = get_cpu_wakeup_source();  // CPU_POWER_RESET
    LOG_USER_MSG_INFO(0, 0,"Wake Up :%u",cpu_status);

	button_press_callback_register(button_press_event);
	ble_mesh_callback_register(ble_mesh_event);
#if LOW_POWER_MODE
	lpn_callback_register(lpn_event);
	store_cpu_status_for_lpn(cpu_status);
#endif
#if NLC_TYPE == BSS_EN
    scene_id_get_callback_register(store_scene_id);
#endif
    store_cpu_status(cpu_status);       // store cpu_status value in button.c file
    provision_status = check_provision_state();

	gpio_en_interrupt(BUTTON_PIN1,1);
	gpio_en_interrupt(BUTTON_PIN2,1);

#if LOW_POWER_MODE
	if(cpu_status == BUTTON_WAKEUP)
	{
        button_read();
    }
#else
#if NLC_TYPE == BSS_EN
    button_press_event(SCENE,SCENE_ID_GET);
#endif
	gpio_set_interrupt(BUTTON_PIN1,POL_FALLING);
	gpio_set_interrupt(BUTTON_PIN2,POL_FALLING);
	irq_enable();
#endif

	if( is_mcu_wakeup_from_deep_retention())
	{
		ble_user_init_deepRetn ();
	}

	else
	{
		ble_init();
#if LOW_POWER_MODE
		if(cpu_status != FACTORY_RESET && !provision_status)
		{
			ble_adv_enable_disable(0);
		}
#endif
	}
#if LOW_POWER_MODE
	 ble_mesh_event(BATTERY_STATUS);
#endif
     while(1)
     {
	   	 hal_watchdog_clear();
	   	 ble_data_processing(); // MD_BATTERY_EN  flash_adr_misc
     }
}

 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/
