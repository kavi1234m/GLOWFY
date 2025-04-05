/**
  **************************
  * @file    hal_nlc_sensor.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    08-Feb-2025
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_NLC_SENSOR_H_
#define HAL_NLC_SENSOR_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vendor/common/nlc/nlc_sensor.h"
#include "vendor/common/sensors_model.h"
#include "vendor/GLOWFY/HAL/hal_ble_mesh.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void hal_nlc_sensor_init(void);
void send_sensor_status(void);

/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_NLC_SENSOR_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
