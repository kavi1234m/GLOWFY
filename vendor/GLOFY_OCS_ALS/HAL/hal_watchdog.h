/**
  **************************
  * @file    hal_watchdog.h
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
#ifndef HAL_WATCHDOG_H_
#define HAL_WATCHDOG_H_


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "drivers/8258/clock.h"
#include "drivers/8258/watchdog.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/*  Functions used to _________________ ***/
 void hal_watchdog_init(void);
 void hal_watchdog_clear(void);
/* Initialization and Configuration functions ***********/



#ifdef __cplusplus
}
#endif

#endif /* HAL_WATCHDOG_H_ */




/***** (C) COPYRIGHT 2022 Calixto Systems Pvt Ltd ***END OF FILE*/
