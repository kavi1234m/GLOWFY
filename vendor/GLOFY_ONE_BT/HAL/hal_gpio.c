/**
  **************************
  * @file    hal_gpio.c
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
#include "hal_gpio.h"
#include "proj/mcu/compiler.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* @brief  interrupt handler
* @param  none
* @retval None
******************************************************************************/
_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler ();  //ble irq proc
}
/******************************************************************************
  * @brief  function1 brief gpio initialization.
  * @param  none
  * @retval None
  ******************************************************************************/
void hal_gpio_init(uint8_t data)
{
       gpio_init(data);

       irq_enable();
}
/******************************************************************************
 * @brief     This function set the pin's output level.
 * @param[in] pin - the pin needs to set its output level
 * @param[in] value - value of the output level(1: high 0: low)
 * @return    none
 ******************************************************************************/
void hal_gpio_write(uint8_t level)
{
	gpio_write(GPIO_PD6,level);
}

/******************************************************************************
 * @brief     This function read the pin's input/output level.
 * @param[in] pin - the pin needs to read its level
 * @return    the pin's level(1: high 0: low)
 ******************************************************************************/
uint8_t hal_gpio_read(void)
{
//	return gpio_read(GPIO_PD6);
	return BM_IS_CLR(reg_gpio_in(GPIO_PD6), GPIO_PD6 & 0xff);
}
/******************************************************************************
 * @brief     This function set the pin toggle.
 * @param[in] pin - the pin needs to toggle
 * @return    none
 ******************************************************************************/
void hal_gpio_toggle(void)
{
	gpio_toggle(GPIO_PD6);
}
/******************************************************************************
 * @brief     This function set the pin as wake up source.
 * @param[in] pin - the pin needs to set as wake up source.
 * @return    none
 ******************************************************************************/
void hal_gpio_set_wakeup_source(void)
{
	gpio_set_wakeup(GPIO_PD6,Level_High,1);
}
/******************************************************************************
 * @brief     This function enable or disable the gpio pin.
 * @param[in] pin - pin number
 * @param[in] enable or disable
 * @return    none
 ******************************************************************************/
void hal_gpio_enable_disable(GPIO_PinTypeDef pin, uint8_t value)
{
	gpio_set_func(pin,value);
}
 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/
