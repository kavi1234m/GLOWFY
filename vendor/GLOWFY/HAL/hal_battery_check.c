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
#include "vendor/GLOWFY/HAL/hal_battery_check.h"

#if LOW_POWER_MODE
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************
 * @brief       convert battery voltage to battery percentage
 * @param[in]   battery in volt
 * @return      none
 ******************************************************************************/
uint8_t convert_battery_volt_to_battery_percentage(u16 battery_mvolt)
{

	uint8_t battery_level;
//	battery_level = ((float)(battery_mvolt - MIN_VOLT)/(MAX_VOLT - MIN_VOLT))*100;
//
//	return (uint8_t)battery_level;

	    if (battery_mvolt >= 3000)
	    {
	        battery_level = 100;
	    }
	    else if (battery_mvolt > 2900)
	    {
	        battery_level = 100 - ((3000 - battery_mvolt) * 58) / 100;
	    }
	    else if (battery_mvolt > 2740)
	    {
	        battery_level = 42 - ((2900 - battery_mvolt) * 24) / 160;
	    }
	    else if (battery_mvolt > 2440)
	    {
	        battery_level = 18 - ((2740 - battery_mvolt) * 12) / 300;
	    }
	    else if (battery_mvolt > 2000)
	    {
	        battery_level = 6 - ((2440 - battery_mvolt) * 6) / 350;
	    }
	    else
	    {
	        battery_level = 0;
	    }

	    return battery_level;

}
/******************************************************************************
 * @brief       send_battery_status function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void send_battery_status(u8 send_data)
{
    u16 pub_addr=0;
    u8 ele_offset = 0;
    model_common_t *p_model;
    u8 model_idx = 0;
    p_model = (model_common_t *)mesh_find_ele_resource_in_model(ele_adr_primary + ele_offset, SIG_MD_G_BAT_S, 1, &model_idx, 0);
	if(p_model && p_model->pub_adr)
	{
		pub_addr = p_model->pub_adr;
	}
	if(pub_addr)
	{
		LOG_USER_MSG_INFO(0,0,"SEND BATTERY STATUS");
		SendOpParaDebug(pub_addr, 0, G_BATTERY_STATUS, (u8*)&send_data, sizeof(send_data));
	}
	else
	{
		LOG_USER_MSG_INFO(0,0,"FAILED TO SEND BATTERY STATUS");
	}
}
/******************************************************************************
 * @brief       battery voltage check function
 * @param[in]   none
 * @return      none
 * @note
 ******************************************************************************/
void fetch_battery_voltage_and_send_battery_status(void)
{
	u16 battery_volt = app_battery_power_check(MIN_VOLT,0);
//	u16 battery_volt = app_battery_power_check_and_sleep_handle(1);
	u8 battery_percentage = convert_battery_volt_to_battery_percentage(battery_volt);
	LOG_USER_MSG_INFO(0,0,"BATTERY_PERCENTAGE :%u",battery_percentage);
	send_battery_status(battery_percentage);
}
#endif


 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

