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
#include "hal_sleep.h"
#include "proj/mcu/compiler.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
void  hal_lpn_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if(!button_pin1_read())
	{
	    LOG_USER_MSG_INFO(0, 0,"Before sleep and wakeup set");
	    cpu_set_gpio_wakeup(GPIO_PD6,0,1);
	//	bls_pm_setWakeupSource(PM_WAKEUP_PAD);
	}
	if(!button_pin2_read())
	{
	    LOG_USER_MSG_INFO(0, 0,"Before sleep and wakeup set");
	    cpu_set_gpio_wakeup(GPIO_PD5,0,1);
	//	bls_pm_setWakeupSource(PM_WAKEUP_PAD);
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
	hal_user_init_peripheral(1);
    lpn_set_poll_ready(); // will call mesh_friend_ship_start_poll() in mesh_friend_ship_proc_LPN().
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
	cpu_long_sleep_wakeup(DEEPSLEEP_MODE_RET_SRAM_LOW32K,(PM_WAKEUP_PAD|PM_WAKEUP_TIMER),(POLL_TIMEOUT)*1000*32);

}
 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/
