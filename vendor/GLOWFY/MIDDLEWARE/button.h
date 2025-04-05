
/**
  **************************
  * @file    hal_button.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    10-12-2024
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BUTTON_H_
#define __BUTTON_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "vendor/common/mesh_lpn.h"
#include "vendor/common/blt_soft_timer.h"
#include "vendor/GLOWFY/HAL/hal_sleep.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
void button_event_check(void);
void button_press_callback_register(void *(callback_fun));
void store_cpu_status(uint8_t status);
void lpn_process_key(u8 e, u8 *p, int n);
void button_init(void);

void button_1_read(void);
void button_2_read(void);
void button_read(void);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
