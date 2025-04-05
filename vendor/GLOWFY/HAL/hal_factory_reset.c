/**
  **************************
  * @file    hal_factory_reset.c
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    23-Jan-2025
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
#include "vendor/GLOWFY/HAL/hal_factory_reset.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/******************************************************************************
  * @brief  function for reset the node
  * @param  None
  * @retval None
  ******************************************************************************/
void node_reset(void)
{
     kick_out(0);// 1-led indication 0-no indication
}


 /***********End of function_demo() function****************/


/***** (C) COPYRIGHT 2024 Calixto Systems Pvt Ltd ***END OF FILE*/

