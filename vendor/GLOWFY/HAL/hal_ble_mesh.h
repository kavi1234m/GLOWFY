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
#include "vendor/GLOWFY/MIDDLEWARE/button.h"
#include "proj_lib/ble/ll/ll.h"
#include "vendor/GLOWFY/HAL/hal_peripheral_init.h"
#include "vendor/GLOWFY/HAL/hal_sleep.h"
#include "vendor/GLOWFY/HAL/hal_battery_check.h"
#include "vendor/common/scene.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void lpn_state_check(uint8_t cpu_status);
void ble_mesh_callback_register(void *(callback_fun));
void send_onoff_command(void);
void brightness_control_using_dimmer(uint8_t type);
void scene_store_and_scene_recall(uint8_t type,uint16_t scene_id);
void reset_device(void);
int app_event_handler (u32 h, u8 *p, int n);
void ble_init(void);
void hal_rf_drv_init(void);
void sig_mesh_start(void);
void ble_entry(void);
void ble_adv_enable_disable(uint8_t status);
uint8_t check_provision_state(void);
void sleep_set(void);
uint16_t check_publish_address(u32 model_id,u8 element_offset);

u32 hal_get_lpn_poll_interval_ms(void);
uint32_t hal_mesh_lpn_adv_interval_update(u8 adv_tick_refresh);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_BLE_MESH_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
