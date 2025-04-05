/**
  **************************
  * @file    device_config.h
  * @author  Kaviyarasan , Calixto Firmware Team
  * @version V1.0.0
  * @date    24-feb-2025
  * @brief   This file contains all the functions prototypes for the __.
  **************************
  *
  * <h2><center>&copy; COPYRIGHT 2024 Calixto Systems Pvt Ltd</center></h2>
  **************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef DEVICE_CONFIG_H_
#define DEVICE_CONFIG_H_


/***********Debug print *****************/
#define  DEBUG_PRINT_ENABLE_DISABLE           0
#define HCI_LOG_FW_EN   (0 || DEBUG_PRINT_ENABLE_DISABLE)
#if HCI_LOG_FW_EN
  #define DEBUG_INFO_TX_PIN           		GPIO_PB2
  #define PRINT_DEBUG_INFO                    1
#endif

/***********RF******/

#define RF_POWER                        RF_POWER_P10p46dBm
//----------------------- GPIO for UI --------------------------------

#define BUTTON_PIN1                     GPIO_PD6
#define BUTTON_PIN2                     GPIO_PD5
#define GPIO_LED	                    GPIO_PA3

/**********LPN config********************/

#define LOW_POWER_MODE                   1        // 1-enable  0-disable
#define MIN_VOLT                         2000
#define MAX_VOLT                         3000

#define PROVISION_TIME                  30   //in seconds
#define POLL_TIMEOUT                   (30*60)   //(1*60)=1 minute
#define KEY_BIND_TIME                   20   //in seconds
#define LONG_PRESS_TIMEOUT              10   //in seconds

/************ nlc profile ************************/

#define  OCS_EN               1
#define  OCS_EN_AND_ALS_EN    2
#define  DIM_EN               3
#define  BSS_EN               4
#define  ENM_EN               5

/************ LOW_POWER_BUTTON *********************/
#define ONE_BT                6

#define NLC_TYPE              DIM_EN


#if NLC_TYPE == OCS_EN
   #define DEVICE_NAME                 "GLOWFY_OCS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif NLC_TYPE == OCS_EN_AND_ALS_EN
   #define DEVICE_NAME                 "GLOWFY_ALS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif NLC_TYPE == DIM_EN
   #define DEVICE_NAME                 "GLOWFY_DIM_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif NLC_TYPE == BSS_EN
   #define DEVICE_NAME                 "GLOWFY_BSS_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif NLC_TYPE == ENM_EN
   #define DEVICE_NAME                 "GLOWFY_ENM_NLC"       //16 character max
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#elif NLC_TYPE == ONE_BT
   #define DEVICE_NAME                 "GLOWFY_ONE_BT"
   #define DEVICE_LEN                  sizeof(DEVICE_NAME)
#endif

//----------------------------ADC--------------------------------------
#define BATT_CHECK_ENABLE       			LOW_POWER_MODE   //1   //must enable
#if (BATT_CHECK_ENABLE)
//telink device: you must choose one gpio with adc function to output high level(voltage will equal to vbat), then use adc to measure high level voltage
	//use PC5 output high level, then adc measure this high level voltage
	#define GPIO_VBAT_DETECT				GPIO_PC5
	#define PC5_FUNC						AS_GPIO
	#define PC5_INPUT_ENABLE				0
	#define ADC_INPUT_PCHN					C5P    //corresponding  ADC_InputPchTypeDef in adc.h C5P
#endif

#define ADC_ENABLE		0
#if ADC_ENABLE
  #define ADC_BASE_MODE	            1	//GPIO voltage
  #define ADC_VBAT_MODE	            2	//Battery Voltage

  #define ADC_MODE		            ADC_VBAT_MODE
  #define ADC_CHNM_ANA_INPUT 		GPIO_PB3 // one of ADC_GPIO_tab[]
  #define ADC_PRESCALER	            ADC_PRESCALER_1F8
#endif

#endif /* DEVICE_CONFIG_H_ */

/***** (C) COPYRIGHT 2025 Calixto Systems Pvt Ltd ***END OF FILE*/
