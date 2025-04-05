
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
/* Private typedef -----------------------------------------------------------*/
typedef void (*btn_fn_cb)();
btn_fn_cb   callback_for_button_press;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
uint32_t press_start_time = 0;
int button_pressed = 0;     // Track whether the button is currently pressed
//uint8_t flag=1;
int long_press_detected = 0;
extern u8 lpn_provision_ok;
extern u8 cpu_status;

enum {
	SINGLE_PRESS  = 0X01,
	LONG_PRESS   =  0X02,
	AFTER_PROVISION_FAILED_KEY_RELEASED =  0X03,
};
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
}
/******************************************************************************
 * @brief       button event check function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void button_event_check(void)
{
	lpn_process_keyboard(0,0,0);

}
/******************************************************************************
  * @brief  button event.
  * @param  param1:wake up flag
  * @param  param2:
  * @retval None
  ******************************************************************************/
void lpn_process_keyboard(u8 e, u8 *p, int n)
{
	uint8_t button_state = hal_gpio_read();

	// Button Pressed
	if (button_state && button_pressed == 0) {  // Button just pressed
	    LOG_USER_MSG_INFO(0, 0, "Button pressed");
	    press_start_time = clock_time();  // Record the press start time
	    button_pressed = 1;               // Mark the button as pressed
	    long_press_detected = 0;          // Reset long press flag
	}
	else if (!button_state && button_pressed == 1)
	{  // Button just released
		    LOG_USER_MSG_INFO(0, 0, "Button Released");
            callback_for_button_press(SINGLE_PRESS);
	        button_pressed = 0;
	}
	if (button_pressed && !long_press_detected && clock_time_exceed(press_start_time,(LONG_PRESS_TIMEOUT*1000*1000))) {
	    // Long press detected
		LOG_USER_MSG_INFO(0, 0, "long press");
	    long_press_detected = 1;  // Mark long press as detected to avoid multiple triggers
	    button_pressed=0;
	    callback_for_button_press(LONG_PRESS);
	}
	if(!check_provision_state() && (cpu_status != 3))
	{
		if(!button_state)
		{
			LOG_USER_MSG_INFO(0, 0, "deep sleep");
			callback_for_button_press(AFTER_PROVISION_FAILED_KEY_RELEASED);
		}
	}

	return;
}

 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/


