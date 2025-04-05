/**
  **************************
  * @file    hal_clock.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    09-12-2024
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_CLOCK_H_
#define HAL_CLOCK_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "drivers/8258/clock.h"
#include "vendor/common/mesh_common.h"
#include "vendor/GLOFY_OCS_ALS/HAL/hal.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void system_clock_config(void);

/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_CLOCK_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
