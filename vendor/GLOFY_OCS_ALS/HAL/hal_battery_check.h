/**
  **************************
  * @file    hal_battery_check.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    31-Jan-2025
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_BATTERY_CHECK_H_
#define HAL_BATTERY_CHECK_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vendor/common/battery_check.h"
#include "vendor/common/generic_model.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
 void fetch_battery_voltage_and_send_status(void);
/*  Functions used to _________________ ***/


/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_BATTERY_CHECK_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
