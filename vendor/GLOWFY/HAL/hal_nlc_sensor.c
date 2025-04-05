/**
  **************************
  * @file    hal_nlc_sensor.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    08-Feb-2025
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
#include "vendor/GLOWFY/HAL/hal_nlc_sensor.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 ocs_value=0;
u32 ambient_value=0;

//extern u8 cmd_success_flag;
//extern u32 wakeup_time;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/********************************************************************************
  * @brief  sensor init
  * @param  None
  * @retval None
  ********************************************************************************/
void hal_nlc_sensor_init(void)
{
    nlc_sensor_init();
}
/********************************************************************************
  * @brief  publisg address check to send sensor status
  * @param  None
  * @retval None
  ********************************************************************************/
u8 publish_address_check(void)
{
    u16 pub_addr=0;
    u8 ele_offset=0;
    model_common_t *p_model;
    u8 model_idx = 0;
    p_model = (model_common_t *)mesh_find_ele_resource_in_model(ele_adr_primary + ele_offset, SIG_MD_SENSOR_S, 1, &model_idx, 0);
	if(p_model && p_model->pub_adr){
		pub_addr = p_model->pub_adr;
	}else{
		pub_addr += ele_offset;
	}
	if(pub_addr)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
/********************************************************************************
  * @brief  occupancy sensor value get
  * @param  None
  * @retval None
  ********************************************************************************/
u8 occupancy_sensor_value_get(void)
{
   ocs_value = !ocs_value;
   return ocs_value;
}

/********************************************************************************
  * @brief  ambient sensor value get
  * @param  None
  * @retval None
  ********************************************************************************/
u32 ambient_sensor_value_get(void)
{
	ambient_value++;
    return ambient_value;
}
/********************************************************************************
  * @brief  send sensor status to light
  * @param  None
  * @retval None
  ********************************************************************************/
void send_sensor_status(void)
{
//	cmd_success_flag=1;
//	wakeup_time = clock_time();
	 u8 ocs_sensor_raw_value = occupancy_sensor_value_get();
    #if ALS_EN
	 u32 ambient_sensor_raw_value = ambient_sensor_value_get();
    #endif
	 store_occupancy_sensor_value(ocs_sensor_raw_value);
    #if ALS_EN
	 store_ambient_sensor_value(ambient_sensor_raw_value);
    #endif
     if(publish_address_check() == 1)
     {
    	 mesh_sensor_st_publish(0);
     }
//     sleep_set();
}

 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

