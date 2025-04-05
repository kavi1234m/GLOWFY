/**
  **************************
  * @file    hal_peripheral_init.h
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
#ifndef HAL_PERIPHERAL_INIT_H_
#define HAL_PERIPHERAL_INIT_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vendor/GLOWFY/HAL/hal_sleep.h"
#include "stack/ble/ll/ll.h"
#include "vendor/GLOWFY/HAL/hal_gpio.h"
#include "vendor/GLOWFY/HAL/hal_clock.h"
#include "vendor/GLOWFY/HAL/hal_watchdog.h"
#include "vendor/GLOWFY/HAL/hal_ble_mesh.h"
#include "vendor/GLOWFY/HAL/hal_adc.h"
#include "proj/mcu/compiler.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void system_init(void);
void hal_user_init_peripheral(int retention_flag);
void hal_peripheral_init(void);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_PERIPHERAL_INIT_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
