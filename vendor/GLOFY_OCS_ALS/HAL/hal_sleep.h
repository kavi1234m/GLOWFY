/**
  **************************
  * @file    hal_sleep.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    07-12-2024
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_SLEEP_H_
#define HAL_SLEEP_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "drivers/8258/pm.h"
#include "vendor/common/mesh_common.h"
#include "vendor/GLOFY_OCS_ALS/HAL/hal_peripheral_init.h"
#include "vendor/GLOFY_OCS_ALS/MIDDLEWARE/button.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void hal_cpu_wakeup_init(void);
void wakeup_source_for_sleep(void);
uint8_t is_mcu_wakeup_from_deep_retention(void);
void  hal_lpn_set_sleep_wakeup (u8 e, u8 *p, int n);
void ble_user_init_deepRetn(void);
void set_deepsleep_without_retention_and_wakeup_by_pad(void);
void set_deepsleep_retention_mode(void);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /*__FILENAME_H */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
