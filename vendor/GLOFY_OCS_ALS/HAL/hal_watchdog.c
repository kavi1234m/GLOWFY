/**
  **************************
  * @file    hal_watchdog.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    10-Dec-2024
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
#include "vendor/GLOFY_OCS_ALS/HAL/hal_watchdog.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


/***********************************************************************************
  * @brief  function for watchdog init.
  * @param  None
  * @retval None
  ***********************************************************************************/
void hal_watchdog_init(void)
{

	 watchdog_init();
}
/***********************************************************************************
  * @brief  function for watchdog clear.
  * @param  None
  * @retval None
  ***********************************************************************************/
void hal_watchdog_clear(void)
{
      wd_clear();
}



 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

