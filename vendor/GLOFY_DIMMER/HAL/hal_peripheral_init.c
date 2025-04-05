/**
  **************************
  * @file    hal_peripheral_init.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    09-12-2024
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
#include "hal_peripheral_init.h"
#include "proj/mcu/compiler.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***********************************************************************************
  * @brief  function for system init.
  * @param  None
  * @retval None
  ***********************************************************************************/
void system_init(void)
{
	wakeup_source_for_sleep();

	hal_cpu_wakeup_init();

	system_clock_config();

}
/***********************************************************************************
  * @brief  function for peripheral initialization.
  * @param  None
  * @retval None
  ***********************************************************************************/
void hal_peripheral_init(void)
{
	uint8_t deepret_wakeup=is_mcu_wakeup_from_deep_retention();

	hal_gpio_init( !deepret_wakeup );

	button_init();

	hal_watchdog_init();

	hal_rf_drv_init();

   #if ADC_ENABLE
	hal_adc_init();
   #endif

}

/***********************************************************************************
  * @brief  function for hal_user_init_peripheral.
  * @param  None
  * @retval None
  ***********************************************************************************/
_attribute_ram_code_ void hal_user_init_peripheral(int retention_flag)
{
	//unprovision:ADV_INTERVAL_MIN;  provision but not friendship:FRI_REQ_TIMEOUT_MS  friendship ok:FRI_POLL_INTERVAL_MS
	if(BLS_LINK_STATE_ADV == blc_ll_getCurrentState()){
		hal_mesh_lpn_adv_interval_update(0);
	}
	button_wakeup_source_set(0,0,0);
//	hal_lpn_set_sleep_wakeup(0,0,0);
}




 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/



