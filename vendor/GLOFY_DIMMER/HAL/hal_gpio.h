/**
  **************************
  * @file    hal_gpio.h
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
#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "drivers/8258/gpio_8258.h"
#include "vendor/common/mesh_lpn.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/

void hal_gpio_init(uint8_t data);
void hal_gpio_write(uint8_t level);
uint8_t button_pin1_read(void);
uint8_t button_pin2_read(void);
void hal_gpio_toggle(void);
void hal_gpio_set_wakeup_source(void);
void hal_gpio_enable_disable(GPIO_PinTypeDef pin, uint8_t value);

/* Initialization and Configuration functions ***********/


#ifdef __cplusplus
}
#endif

#endif /*__FILENAME_H */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
