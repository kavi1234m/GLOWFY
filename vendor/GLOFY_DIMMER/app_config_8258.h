/********************************************************************************************************
 * @file	app_config_8258.h
 *
 * @brief	for TLSR chips
 *
 * @author	telink
 * @date	Sep. 30, 2010
 *
 * @par     Copyright (c) 2017, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/
//#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

#include "../common/version.h"    // include mesh_config.h inside.

//////////////////board sel/////////////////////////////////////
#define PCBA_8258_DONGLE_48PIN          1
#define PCBA_8258_C1T139A30_V1_0        2
#define PCBA_8258_C1T139A30_V1_2        3
#define PCBA_8258_C1T140A3_V1_1         4   // 32pin

#define PCBA_8258_SEL			PCBA_8258_DONGLE_48PIN

#define FLASH_1M_ENABLE                 0

#define APP_FLASH_PROTECTION_ENABLE     1

/***********Debug print *****************/
#define  DEBUG_PRINT_ENABLE_DISABLE           1
#define HCI_LOG_FW_EN   (0 || DEBUG_LOG_SETTING_DEVELOP_MODE_EN)
#if HCI_LOG_FW_EN
  #define DEBUG_INFO_TX_PIN           		GPIO_PB2
  #define PRINT_DEBUG_INFO                    1
#endif

/************ nlc profile************************/
//#define  OCS_EN             0
//#define  ALS_EN             0
//#define  DIM_EN             1
//#define  BSS_EN             0
//#define  ENM_EN             0

#if OCS_EN
   #define DEVICE_NAME                 "GLOFY_OCS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif ALS_EN
   #define DEVICE_NAME                 "GLOFY_ALS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif DIM_EN
   #define DEVICE_NAME                 "GLOFY_DIM_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif BSS_EN
   #define DEVICE_NAME                 "GLOFY_BSS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif ENM_EN
   #define DEVICE_NAME                 "GLOFY_ENM_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#endif


/////////////////// MODULE /////////////////////////////////
#define BLE_REMOTE_PM_ENABLE			1
#define PM_DEEPSLEEP_RETENTION_ENABLE   1   // must
#define BLT_SOFTWARE_TIMER_ENABLE		1

/**********LPN config********************/
#define PROVISION_TIME                  30   //in seconds
#define POLL_TIMEOUT                   (30*60)   //(1*60)=1 minute
#define KEY_BIND_TIME                   30   //in seconds
#define LONG_PRESS_TIMEOUT              10   //in seconds
/***********RF******/
#define RF_POWER                        RF_POWER_P10p46dBm
//----------------------- GPIO for UI --------------------------------

#define UI_KEYBOARD_ENABLE				1
#define BUTTON_PIN1                     GPIO_PD6
#define BUTTON_PIN2                     GPIO_PD5
#define GPIO_LED	                    GPIO_PA3

//----------------------------ADC--------------------------------------
#define BATT_CHECK_ENABLE       			1   //must enable
#if (BATT_CHECK_ENABLE)
//telink device: you must choose one gpio with adc function to output high level(voltage will equal to vbat), then use adc to measure high level voltage
	//use PC5 output high level, then adc measure this high level voltage
	#define GPIO_VBAT_DETECT				GPIO_PB4
	#define PB4_FUNC						AS_GPIO
	#define PB4_INPUT_ENABLE				0
	#define ADC_INPUT_PCHN					B4P    //corresponding  ADC_InputPchTypeDef in adc.h
#endif

#define ADC_ENABLE		0
#if ADC_ENABLE
  #define ADC_BASE_MODE	            1	//GPIO voltage
  #define ADC_VBAT_MODE	            2	//Battery Voltage

  #define ADC_MODE		            ADC_BASE_MODE
  #define ADC_CHNM_ANA_INPUT 		GPIO_PB3 // one of ADC_GPIO_tab[]
  #define ADC_PRESCALER	            ADC_PRESCALER_1F8
#endif

/////////////open SWS digital pullup to prevent MCU err, this is must ////////////
#define PA7_DATA_OUT			1

/////////////////// Clock  /////////////////////////////////
#define	USE_SYS_TICK_PER_US
#define CLOCK_SYS_TYPE  		CLOCK_TYPE_PLL	//  one of the following:  CLOCK_TYPE_PLL, CLOCK_TYPE_OSC, CLOCK_TYPE_PAD, CLOCK_TYPE_ADC

#define CLOCK_SYS_CLOCK_HZ  	32000000

/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		1
#define WATCHDOG_INIT_TIMEOUT		2000  //ms


/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
