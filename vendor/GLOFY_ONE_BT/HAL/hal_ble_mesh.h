/**
  **************************
  * @file    hal_ble_mesh.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    10-Dec-2024
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_BLE_MESH_H_
#define HAL_BLE_MESH_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
//#include "vendor/common/mesh_lpn.h"
//#include "vendor/common/blt_soft_timer.h"
#include "vendor/GLOFY_ONE_BT/MIDDLEWARE/button.h"
#include "proj_lib/ble/ll/ll.h"
#include "vendor/GLOFY_ONE_BT/HAL/hal_peripheral_init.h"
#include "vendor/GLOFY_ONE_BT/HAL/hal_sleep.h"
#include "vendor/GLOFY_ONE_BT/HAL/hal_battery_check.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void lpn_state_check(void);
void ble_mesh_callback_register(void *(callback_fun));
void send_onoff_command(void);
void reset_device(void);
int app_event_handler (u32 h, u8 *p, int n);
void ble_init(void);
void hal_rf_drv_init(void);
void sig_mesh_start(void);
void ble_entry(void);
void ble_adv_enable_disable(uint8_t status);
uint8_t check_provision_state(void);

u32 hal_get_lpn_poll_interval_ms(void);
uint32_t hal_mesh_lpn_adv_interval_update(u8 adv_tick_refresh);
void button_wakeup_source_set(u8 e, u8 *p, int n);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_BLE_MESH_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
