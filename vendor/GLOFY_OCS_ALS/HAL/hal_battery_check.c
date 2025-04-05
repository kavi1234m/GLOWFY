/**
  **************************
  * @file    hal_battery_check.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    31-Jan-2025
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
#include "vendor/GLOFY_OCS_ALS/HAL/hal_battery_check.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
 * @brief       send_battery_status function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void send_battery_status(u16 send_data)
{
    u16 pub_addr=0;
    u8 ele_offset=0;
    model_common_t *p_model;
    u8 model_idx = 0;
    p_model = (model_common_t *)mesh_find_ele_resource_in_model(ele_adr_primary + ele_offset, SIG_MD_G_BAT_S, 1, &model_idx, 0);
	if(p_model && p_model->pub_adr){
		pub_addr = p_model->pub_adr;
	}else{
		pub_addr += ele_offset;
	}
	if(pub_addr)
	{
		SendOpParaDebug(pub_addr, 0, G_BATTERY_STATUS, (u8*)&send_data, sizeof(send_data));
	}
}
/******************************************************************************
 * @brief       battery voltage check function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void fetch_battery_voltage_and_send_status(void)
{
	u16 battery_volt=app_battery_power_check_and_sleep_handle(1);
	send_battery_status(battery_volt);
}


 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

