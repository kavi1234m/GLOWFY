/**
  **************************
  * @file    hal_led.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    12-Dec-2024
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef HAL_LED_H_
#define HAL_LED_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vendor/common/light.h"
#include "drivers/8258/gpio_8258.h"
#include "vendor/GLOFY_ONE_BT/HAL/hal_gpio.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void led_event(void);
void led_1_sec_on_2_sec_off_30_sec(void);
void light_off(void);
void led_enable_disable(uint8_t level);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_LED_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
