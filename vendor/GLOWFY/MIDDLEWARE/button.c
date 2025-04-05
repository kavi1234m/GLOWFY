
/**
  **************************
  * @file    button.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    11-12-2024
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
#include "button.h"
#include "vendor/common/scene.h"
/* Private typedef -----------------------------------------------------------*/
typedef void (*btn_fn_cb)();
btn_fn_cb   callback_for_button_press;
btn_fn_cb   callback_for_dimmer_control;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
uint32_t press_start_time = 0;
uint8_t button_1_pressed = 0;     // Track whether the button is currently pressed

uint32_t press_2_start_time = 0;
uint8_t button_2_pressed = 0;     // Track whether the button is currently pressed

uint8_t button_1_state =0;
uint8_t button_2_state =0;

uint8_t id_get_flag = 0;

uint8_t send_cmd_on_button_1_released = 0;
uint8_t send_cmd_on_button_2_released = 0;

uint8_t dimmer_increase_flag = 0;
uint8_t dimmer_decrease_flag = 0;

u8 cpu_status;
/*********cpu wakeup source*************/

#define POWER_UP              0x00
#define WATCHDOG_RESET        0x01
#define BUTTON_WAKEUP         0x02
#define FACTORY_RESET         0x03
/************event type*************/
#define PRESS                          1
#define DIMMER                         2
#define SCENE                          3
/***button event***********/
#define SINGLE_PRESS                   11
#define LONG_PRESS                     12
#define BEFORE_PROVISION_SLEEP         13
/******dimmer control event*********/
#define BRIGHTNESS_INCREASE            21
#define BRIGHTNESS_DECREASE            22
#define BRIGHTNESS_STOP                23

/*********** scene ***********/
#define SCENE_RECALL_WITH_ID_INCREASE  31
#define SCENE_RECALL_WITH_ID_DECREASE  32
#define SCENE_ID_GET                   33

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
  * @brief  function for registering the single press and long press callback function.
  * @param  None
  * @retval None
  ******************************************************************************/
void button_press_callback_register(void *(callback_fun))
{
	callback_for_button_press = callback_fun;
}

/******************************************************************************
  * @brief  function for store cpu status .
  * @param  None
  * @retval None
  ******************************************************************************/
void store_cpu_status(uint8_t status)
{
     cpu_status = status;
}
/******************************************************************************
 * @brief       This function servers initialize the button
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void button_init(void)
{
    gpio_set_func(BUTTON_PIN1,AS_GPIO);
	gpio_set_output_en(BUTTON_PIN1, 0);
    gpio_set_input_en(BUTTON_PIN1,1);
    gpio_setup_up_down_resistor(BUTTON_PIN1,PM_PIN_PULLUP_10K);

#if (NLC_TYPE == DIM_EN ||NLC_TYPE == BSS_EN )
    gpio_set_func(BUTTON_PIN2,AS_GPIO);
	gpio_set_output_en(BUTTON_PIN2, 0);
    gpio_set_input_en(BUTTON_PIN2,1);
    gpio_setup_up_down_resistor(BUTTON_PIN2,PM_PIN_PULLUP_10K);
#endif
}
/******************************************************************************
  * @brief  function Reading the button pin1 status .
  * @param  None
  * @retval None
  ******************************************************************************/
void button_1_read(void)
{
	button_1_state = button_pin1_read();

	if (button_1_state && button_1_pressed == 0)
	{
		LOG_USER_MSG_INFO(0, 0, "Button 1 pressed");
	    press_start_time = clock_time();  // Record the press start time
	    button_1_pressed = 1;               // Mark the button as pressed
	    id_get_flag = 0;
	    dimmer_increase_flag = 1;
	}

	else if (!button_1_state && button_1_pressed == 1)
	{
		    button_1_pressed = 0;
		    LOG_USER_MSG_INFO(0, 0, "Button 1 Released");
		    if(check_provision_state() && !id_get_flag)
		    {
		    	send_cmd_on_button_1_released = 1;
		    }
       #if LOW_POWER_MODE
	        if(check_provision_state()&& !button_pin2_read())
	        {
	        	sleep_set();
	        }
       #endif
	}
}
/******************************************************************************
  * @brief  function Reading the button pin2 status .
  * @param  None
  * @retval None
  ******************************************************************************/
void button_2_read(void)
{
	button_2_state = button_pin2_read();

	if (button_2_state && button_2_pressed == 0)
	{
		LOG_USER_MSG_INFO(0, 0, "Button 2 pressed");
	    press_2_start_time = clock_time();  // Record the press start time
	    button_2_pressed = 1;               // Mark the button as pressed
	    id_get_flag = 0;
	    dimmer_decrease_flag = 1;
	}


	else if (! button_2_state && button_2_pressed == 1)
	{
		    button_2_pressed = 0;
	        LOG_USER_MSG_INFO(0, 0, "Button 2 Released");
		    if(check_provision_state() && !id_get_flag)
		    {
		    	send_cmd_on_button_2_released = 1;
		    }
       #if LOW_POWER_MODE
	        if(check_provision_state() && !button_pin1_read())
	        {
	        	sleep_set();
	        }
       #endif
	}

}
/******************************************************************************
  * @brief  button event.
  * @param  None
  * @retval None
  ******************************************************************************/
void button_event_check(void)
{

#if (NLC_TYPE == DIM_EN || NLC_TYPE == BSS_EN)
	if(button_1_pressed && button_2_pressed)
	{
#if (NLC_TYPE == BSS_EN)
		id_get_flag = 2;
		if(clock_time_exceed(press_start_time,2*1000*1000) && clock_time_exceed(press_2_start_time,2*1000*1000))
		{
			 id_get_flag = 1;
		}
#endif
		if(clock_time_exceed(press_start_time,LONG_PRESS_TIMEOUT*1000*1000) && clock_time_exceed(press_2_start_time,LONG_PRESS_TIMEOUT*1000*1000))
		{
			  button_1_pressed=0;
			  button_2_pressed=0;
			  callback_for_button_press(PRESS,LONG_PRESS);
		}

	}
#if NLC_TYPE == BSS_EN
	if(id_get_flag == 1 && check_provision_state())
	{
	     if(!button_1_pressed && !button_2_pressed)
	     {
		     LOG_USER_MSG_INFO(0, 0, "id get command");
		     id_get_flag = 0;
		     callback_for_button_press(SCENE,SCENE_ID_GET);
	      }
	}
#endif
#endif
#if NLC_TYPE == DIM_EN
   if(dimmer_increase_flag)
   {
	   dimmer_increase_flag = 0;
		if(!button_pin2_read() && check_provision_state())
		{
			callback_for_button_press(DIMMER,BRIGHTNESS_INCREASE);
		}
   }
   if(dimmer_decrease_flag)
   {
	   dimmer_decrease_flag = 0;
		if(!button_pin1_read() && check_provision_state())
		{
			callback_for_button_press(DIMMER,BRIGHTNESS_DECREASE);
		}
   }
#endif
   if(send_cmd_on_button_1_released)
   {
	   send_cmd_on_button_1_released = 0;
       #if NLC_TYPE == BSS_EN
	      LOG_USER_MSG_INFO(0, 0, "Scene Recall 1");
          callback_for_button_press(SCENE,SCENE_RECALL_WITH_ID_INCREASE);
       #endif
       #if NLC_TYPE == DIM_EN
           callback_for_button_press(DIMMER,BRIGHTNESS_STOP);
       #endif
       #if (NLC_TYPE == OCS_EN ||NLC_TYPE == OCS_EN_AND_ALS_EN ||NLC_TYPE == ONE_BT )
             callback_for_button_press(PRESS,SINGLE_PRESS);
       #endif
   }

#if (NLC_TYPE ==DIM_EN || NLC_TYPE == BSS_EN)
   if(send_cmd_on_button_2_released)
   {
	   send_cmd_on_button_2_released =0;
    #if NLC_TYPE == BSS_EN
   	   LOG_USER_MSG_INFO(0, 0, "Scene Recall 2");
       callback_for_button_press(SCENE,SCENE_RECALL_WITH_ID_DECREASE);
    #endif
    #if NLC_TYPE == DIM_EN
	    callback_for_button_press(DIMMER,BRIGHTNESS_STOP);
    #endif
    #if !(NLC_TYPE == DIM_EN || NLC_TYPE == BSS_EN )
	    callback_for_button_press(PRESS,SINGLE_PRESS);
    #endif
   }
#endif

#if LOW_POWER_MODE
	if(!check_provision_state() && (cpu_status != FACTORY_RESET))
	{
		if(!button_pin1_read() && !button_pin2_read())
		{
			LOG_USER_MSG_INFO(0, 0, "Before Sleep");
			callback_for_button_press(PRESS,BEFORE_PROVISION_SLEEP);
		}
	}
#endif

}
/******************************************************************************
  * @brief  Read Button 1 and Button 2 and set interrupt polarity.
  * @param  None
  * @retval None
  ******************************************************************************/
void button_read(void)
{
	button_1_read();
#if (NLC_TYPE ==DIM_EN || NLC_TYPE == BSS_EN)
	button_2_read();
#endif
	if(button_pin1_read())
	{
		gpio_set_interrupt_pol(BUTTON_PIN1, POL_RISING);
	}
	else
	{
		gpio_set_interrupt_pol(BUTTON_PIN1, POL_FALLING);
	}
#if (NLC_TYPE == DIM_EN || NLC_TYPE == BSS_EN)
	if(button_pin2_read())
	{
		gpio_set_interrupt_pol(BUTTON_PIN2, POL_RISING);
	}
	else
	{
		gpio_set_interrupt_pol(BUTTON_PIN2, POL_FALLING);
	}
#endif
    irq_enable();
}
 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/


