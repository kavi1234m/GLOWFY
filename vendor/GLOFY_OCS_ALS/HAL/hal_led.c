/**
  **************************
  * @file    hal_led.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    12-Dec-2024
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
#include "vendor/GLOFY_OCS_ALS/HAL/hal_led.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/******************************************************************************
  * @brief  function for execution of led events
  * @param  param1: None
  * @retval None
  ******************************************************************************/
 void led_event(void)
{
		 proc_led();

}
 /***********************************************************************************
   * @brief  led_1_sec_on_2_sec_off_30_sec for blink led 1 sec on 2 sec off during advertising time 30 seconds
   * @param  param1: None
   * @retval None
   ***********************************************************************************/
void led_1_sec_on_2_sec_off_30_sec(void)
{
	cfg_led_event(LED_EVENT_1SEC_ON_2SEC_OFF_30S);
}
/***********************************************************************************
  * @brief  turn off the light
  * @param  param1: None
  * @retval None
  ***********************************************************************************/
void light_off(void)
{
	led_onoff_gpio(GPIO_LED,0);
}
/***********************************************************************************
  * @brief  led enable or disable
  * @param  param1: None
  * @retval None
  ***********************************************************************************/
void led_enable_disable(uint8_t level)
{
	hal_gpio_enable_disable(GPIO_PA3,level);
}



 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

