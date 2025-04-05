/**
  **************************
  * @file    hal_adc.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    30-Jan-2025
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
#include "vendor/GLOFY_OCS_ALS/HAL/hal_adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/******************************************************************************
 * @brief       ADC init function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void hal_adc_init(void)
{
    	adc_drv_init();
}
/******************************************************************************
 * @brief       battery voltage get function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
u16 battery_volt_get(void)
{
	static u32 adc_check_time;
	static u16 T_adc_val;
//    if(clock_time_exceed(adc_check_time, 1000*1000))
//    {
        adc_check_time = clock_time();
		app_battery_check_and_re_init_user_adc();
		T_adc_val = adc_sample_and_get_result();
//    }
    return T_adc_val;
}



 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

