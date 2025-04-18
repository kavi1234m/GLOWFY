/********************************************************************************************************
 * @file	default_config.h
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
#pragma once

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif


#include "../config.h"

//////////// product  Information  //////////////////////////////
#ifndef ID_VENDOR
#define ID_VENDOR			0x248a			// for report
#endif
#ifndef ID_PRODUCT_BASE
#define ID_PRODUCT_BASE		0x8800
#endif
#ifndef STRING_VENDOR
#define STRING_VENDOR		L"Telink"
#endif

#ifndef ACL_PERIPHR_MAX_NUM
#define ACL_PERIPHR_MAX_NUM	1
#endif

#if (__TLSR_RISCV_EN__)
#ifndef STRING_PRODUCT
#define STRING_PRODUCT		L"BLE 5.3"
#endif

#ifndef STRING_SERIAL
#define STRING_SERIAL		L"TLSR95XX"
#endif

#ifndef PM_DEEPSLEEP_RETENTION_ENABLE
#define PM_DEEPSLEEP_RETENTION_ENABLE				0
#endif

#if (PM_DEEPSLEEP_RETENTION_ENABLE)
	#define	BLC_PM_EN								1
	#define	BLC_PM_DEEP_RETENTION_MODE_EN			1
#endif

#ifndef	BLC_PM_EN
#define	BLC_PM_EN									0
#endif

#ifndef	BLC_PM_DEEP_RETENTION_MODE_EN
#define	BLC_PM_DEEP_RETENTION_MODE_EN				0
#endif

#ifndef APP_HW_FIRMWARE_ENCRYPTION_ENABLE
#define APP_HW_FIRMWARE_ENCRYPTION_ENABLE			0 	//firmware Encryption
#endif

#ifndef APP_HW_SECURE_BOOT_ENABLE
#define APP_HW_SECURE_BOOT_ENABLE					0 	//secure boot: firmware signature verification
#endif


#ifndef ACL_CENTRAL_SMP_ENABLE
#define ACL_CENTRAL_SMP_ENABLE						0
#endif


#ifndef BLE_OTA_SERVER_ENABLE
#define BLE_OTA_SERVER_ENABLE						0
#endif




#if (APP_HW_FIRMWARE_ENCRYPTION_ENABLE && !HARDWARE_FIRMWARE_ENCRYPTION_SUPPORT_EN)
	#error "MCU do not support HW FIRMWARE ENCRYPTION"
#endif

#if (APP_HW_SECURE_BOOT_ENABLE && !HARDWARE_SECURE_BOOT_SUPPORT_EN)
	#error "MCU do not support HW SECURE BOOT"
#endif

#else

#ifndef STRING_PRODUCT
#define STRING_PRODUCT		L"2.4G Wireless Audio"
#endif
#ifndef STRING_SERIAL
#define STRING_SERIAL		L"TLSR8869"
#endif
#endif

#define	CHIP_REVISION_A4 	1
#define	CHIP_REVISION_A5 	2

#ifndef CHIP_REVISION
#define	CHIP_REVISION		CHIP_REVISION_A4
#endif

#ifndef DONLGE_MONITOR_MODE
#define DONLGE_MONITOR_MODE	0
#endif

// default setting
#if((0 == __TLSR_RISCV_EN__) && FLASH_1M_ENABLE)
#define FLASH_PLUS_ENABLE		FLASH_1M_ENABLE
#endif
#ifndef FLASH_PLUS_ENABLE
#define FLASH_PLUS_ENABLE	    0	// means 2M flash for B91, 1M flash for B85m
#endif
#if(__TLSR_RISCV_EN__)
#define FLASH_SIZE_MAX_SW		((FLASH_PLUS_ENABLE ? 2048 : 1024) * 1024)
#else
#define FLASH_SIZE_MAX_SW		((FLASH_PLUS_ENABLE ? 1024 : 512) * 1024)
#endif

#ifndef PINGPONG_OTA_DISABLE
#define PINGPONG_OTA_DISABLE    0
#endif
#ifndef	SWITCH_FW_ENABLE
#define SWITCH_FW_ENABLE		0
#endif
#ifndef	GATT_LPN_EN
#define GATT_LPN_EN		        0
#endif
#ifndef	MESH_DLE_MODE
#define MESH_DLE_MODE		    0
#endif
//#ifndef	EXTENDED_ADV_ENABLE
//#define EXTENDED_ADV_ENABLE		0
//#endif
#ifndef	DUAL_OTA_NEED_LOGIN_EN
#define DUAL_OTA_NEED_LOGIN_EN	0
#endif
#ifndef	ENCODE_OTA_BIN_EN
#define ENCODE_OTA_BIN_EN      	0
#endif
#ifndef	DISTRIBUTOR_UPDATE_CLIENT_EN
#define DISTRIBUTOR_UPDATE_CLIENT_EN		0
#endif
#ifndef	DISTRIBUTOR_UPDATE_SERVER_EN
#define DISTRIBUTOR_UPDATE_SERVER_EN      	0
#endif
#ifndef	APP_LOG_EN
#define APP_LOG_EN      		0
#endif
#ifndef	SPEECH_ENABLE
#define SPEECH_ENABLE      		0
#endif
#ifndef	AUDIO_MESH_EN
#define AUDIO_MESH_EN      		0
#endif
#ifndef PAIR_PROVISION_ENABLE
#define PAIR_PROVISION_ENABLE	0
#endif
#ifndef	USE_DP_TO_BURN_FW
#define USE_DP_TO_BURN_FW      	0
#endif

#if __PROJECT_MESH_PRO__
#define MESH_BLE_NOTIFY_FIFO_EN     0
#else
#define MESH_BLE_NOTIFY_FIFO_EN     (MESH_DLE_MODE != MESH_DLE_MODE_EXTEND_BEAR)
#endif

#ifndef LIGHT_CONTROL_MODEL_LOCATE_PRIMARY_ELEMENT
#define LIGHT_CONTROL_MODEL_LOCATE_PRIMARY_ELEMENT		0
#endif

#ifndef LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN
#define LIGHT_CONTROL_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN	0
#endif

//////////// debug  /////////////////////////////////
#ifndef __DEBUG__
#define	__DEBUG__			0		//  to enable assert
#endif
#ifndef __DEBUG_PRINT__
#define	__DEBUG_PRINT__		0		//  to enable printf
#endif
#ifndef __SHOW_TODO__
#define	__SHOW_TODO__		1		//  to remind todo thing
#endif
#ifndef __SHOW_WARN__
#define	__SHOW_WARN__		1		//  to remind warning thing
#endif
#ifndef __SHOW_NOTE__
#define	__SHOW_NOTE__		1		//  to remind note thing
#endif

#ifndef __LOG_RT_ENABLE__
#define __LOG_RT_ENABLE__	0		//  readtime vcd log enable
#endif

#ifndef __MOUSE_SIMU__
#define __MOUSE_SIMU__  	0
#endif
#ifndef __KEYBOARD_SIMU__
#define __KEYBOARD_SIMU__  	0
#endif

#ifndef __RF_SIMU__
#define __RF_SIMU__  		0
#endif
#ifndef __ETH_SIMU__
#define __ETH_SIMU__  		0
#endif

/////////////////// MODULE /////////////////////////////////
#ifndef MODULE_AUDIO_ENABLE
#define MODULE_AUDIO_ENABLE		0
#endif
#ifndef MODULE_SBC_ENABLE
#define MODULE_SBC_ENABLE		0
#endif
#ifndef MODULE_ADC_ENABLE
#define MODULE_ADC_ENABLE		0
#endif
#ifndef MODULE_WATCHDOG_ENABLE
#define MODULE_WATCHDOG_ENABLE	0
#endif
#ifndef MODULE_RF_PA_ENABLE
#define MODULE_RF_PA_ENABLE		0
#endif
#ifndef MODULE_RF_ENABLE
#define MODULE_RF_ENABLE		1
#endif
#ifndef MODULE_PM_ENABLE
#define MODULE_PM_ENABLE		0			//  power management
#endif
#ifndef MODULE_ETH_ENABLE
#define MODULE_ETH_ENABLE		0
#endif
#ifndef MODULE_SOMATIC_ENABLE
#define MODULE_SOMATIC_ENABLE	0
#endif
#ifndef MODULE_OFN_ENABLE
#define MODULE_OFN_ENABLE		0
#endif

#ifndef KB_HAS_NUMPAD
#define KB_HAS_NUMPAD 			0
#endif

#ifndef WHEEL_KB_ENABLE
#define WHEEL_KB_ENABLE 		0
#endif

#ifndef MODULE_LED_ENABLE
#define MODULE_LED_ENABLE		0
#endif

#ifndef MODULE_BATT_ENABLE				// battery check
#define MODULE_BATT_ENABLE		0
#endif

#ifndef MODULE_DMIC_ENABLE
#define MODULE_DMIC_ENABLE		0
#endif

#if(APPLICATION_DONGLE)
#if(MCU_CORE_TYPE == MCU_CORE_3520)
	#ifndef MODULE_KEYBOARD_ENABLE
	#define MODULE_KEYBOARD_ENABLE	1
	#endif
#endif
	#ifndef MODULE_MOUSE_ENABLE
	#define MODULE_MOUSE_ENABLE		0
	#endif
	#ifndef MODULE_KEYBOARD_ENABLE
	#define MODULE_KEYBOARD_ENABLE	0
	#endif
	#ifndef MODULE_MIC_ENABLE
	#define MODULE_MIC_ENABLE		0
	#endif
	#ifndef MODULE_SPEAKER_ENABLE
	#define MODULE_SPEAKER_ENABLE	0			// device , not dongle
	#endif
	#ifndef MODULE_USB_ENABLE
	#define MODULE_USB_ENABLE		1
	#endif
#else
	#ifndef MODULE_MOUSE_ENABLE
	#define MODULE_MOUSE_ENABLE		1
	#endif
	#ifndef MODULE_KEYBOARD_ENABLE
	#define MODULE_KEYBOARD_ENABLE	1
	#endif

	#ifndef MODULE_MIC_ENABLE
	#define MODULE_MIC_ENABLE		0
	#endif
	#ifndef MODULE_SPEAKER_ENABLE
	#define MODULE_SPEAKER_ENABLE	0			// device , not dongle
	#endif

	#ifndef MODULE_USB_ENABLE
	#define MODULE_USB_ENABLE		0
	#endif
#endif

#ifndef UART_8266_EN
#define UART_8266_EN				0
#endif

#ifndef MOUSE_HAS_BUTTON
#define MOUSE_HAS_BUTTON			0
#endif

#ifndef MOUSE_DIRECTION
#define MOUSE_DIRECTION			SENSOR_IDX_CLOCK_3
#endif

#ifndef	DIRC_LOAD_DEEPSLEEP
#define DIRC_LOAD_DEEPSLEEP			0
#endif

#ifndef MOUSE_DIR_R_SELECT_WCX
#define MOUSE_DIR_R_SELECT_WCX		0
#endif

#ifndef TX_FAST_MODE_ENABLE
#define	TX_FAST_MODE_ENABLE			0
#endif

#ifndef MOUSE_WHEEL_DIRECTION
#define MOUSE_WHEEL_DIRECTION		0
#endif

#ifndef	FLAG_BUTTON_LEFT
#define FLAG_BUTTON_LEFT		0x01
#endif

#ifndef	FLAG_BUTTON_RIGHT
#define FLAG_BUTTON_RIGHT		0x02
#endif

#ifndef	FLAG_BUTTON_MIDDLE
#define FLAG_BUTTON_MIDDLE		0x04
#endif

#ifndef	FLAG_BUTTON_FORWARD
#define FLAG_BUTTON_FORWARD		0x10
#endif

#ifndef	FLAG_BUTTON_BACKWARD
#define FLAG_BUTTON_BACKWARD	0x08
#endif

#ifndef	FLAG_BUTTON_DPI
#define FLAG_BUTTON_DPI			0x20
#endif

#ifndef	FLAG_BUTTON_LMRFB
#define FLAG_BUTTON_LMRFB		0x0f
#endif

#ifndef FLAG_BUTTON_ONEKEYWEB
#define FLAG_BUTTON_ONEKEYWEB    0x40
#endif

#ifndef	FLAG_BUTTON_TEST_MODE
#define FLAG_BUTTON_PARING	(FLAG_BUTTON_LEFT | FLAG_BUTTON_RIGHT)
#endif

#ifndef	FLAG_BUTTON_TEST_MODE
#define FLAG_BUTTON_EMI		(FLAG_BUTTON_LEFT | FLAG_BUTTON_RIGHT | FLAG_BUTTON_MIDDLE)
#endif

#ifndef MOUSE_HAS_WHEEL
#define MOUSE_HAS_WHEEL				0
#endif

#ifndef	MODULE_KM_LED
#define	MODULE_KM_LED				0
#endif

#ifndef	KM_LED_LEVEL
#define	KM_LED_LEVEL			0
#endif

#ifndef	KM_LED_PIN
#define	KM_LED_PIN			0
#endif

#ifndef	KM_LED_BATTERY_LOW_TO_SUSPEND
#define	KM_LED_BATTERY_LOW_TO_SUSPEND		0
#endif

#ifndef	PARING_ENABLE_ON_POWER
#define	PARING_ENABLE_ON_POWER		0
#endif

#ifndef	MOUSE_DIRECTION_PIN0
#define	MOUSE_DIRECTION_PIN0		GPIO_MSDO
#endif

#ifndef	MOUSE_DIRECTION_PIN1
#define	MOUSE_DIRECTION_PIN1		GPIO_MSDI
#endif

#ifndef ULTRA_LOW_POWER
#define ULTRA_LOW_POWER				0
#endif

#ifndef	CUSTOM_MOUSE_DIR_R_SELECT
#define	CUSTOM_MOUSE_DIR_R_SELECT		0
#endif

#ifndef WATCHDOG_INIT_TIMEOUT
#define WATCHDOG_INIT_TIMEOUT		200		//  in ms
#endif

#ifndef DCDC_SUPPLY_VOLTAGE
#define	DCDC_SUPPLY_VOLTAGE			ll_ldo_set_with_bat_2p8_to_3p0
#endif

#ifndef BATTERY_LOW_INDEX
#define	BATTERY_LOW_INDEX			ll_ldo_set_with_bat_2p0_to_2p25
#endif

#ifndef DCDC_SUPPLY_VOLTAGE_A3000
#define DCDC_SUPPLY_VOLTAGE_A3000	ll_ldo_set_with_bat_2p0_to_2p25
#endif

#ifndef BATTERY_DETECTION_AND_LDO_SET_AUTO
#define BATTERY_DETECTION_AND_LDO_SET_AUTO	0
#endif

#ifndef BATTERY_DETECTION_WITH_LDO_SET
#define	BATTERY_DETECTION_WITH_LDO_SET	1
#endif

#ifndef	PARING_MANUAL_MODE_ENABLE
#define	PARING_MANUAL_MODE_ENABLE				1
#endif

#ifndef	PARING_AUTO_MODE_ENABLE
#define	PARING_AUTO_MODE_ENABLE				0
#endif

#ifndef	PARING_POWERON_MODE_ENABLE
#define	PARING_POWERON_MODE_ENABLE				0
#endif

#ifndef		WAKEUP_SEARCH_MODE
#define		WAKEUP_SEARCH_MODE		0
#endif

#ifndef	POWERON_LONG_DELAY_1
#define	POWERON_LONG_DELAY_1		0
#endif

#ifndef	MOUSE_3204_ONLY
#define	MOUSE_3204_ONLY				0
#endif

#ifndef SENSOR_PL_8204_SUPPORT
#define SENSOR_PL_8204_SUPPORT		0
#endif

#ifndef	MOUSE_A3000_ONLY
#define	MOUSE_A3000_ONLY			0
#endif

#ifndef NEW_DPI_CONFIG
#define NEW_DPI_CONFIG              0
#endif

#ifndef NEW_DPI_FULL_CUST
#define NEW_DPI_FULL_CUST           0
#endif


#ifndef LR_SW_CPI
#define LR_SW_CPI                   0
#endif

#ifndef	NO_DETECT_MOTION
#define NO_DETECT_MOTION			0
#endif

#ifndef MOV_PIN_DSCNNCT
#define MOV_PIN_DSCNNCT				0
#endif

#ifndef MOUSE_BUTTON_DEBOUNCE
#define MOUSE_BUTTON_DEBOUNCE		0
#endif

///////////////////  interrupt  //////////////////////////////
#ifndef IRQ_TIMER1_ENABLE
#define	IRQ_TIMER1_ENABLE  	0
#endif
#ifndef IRQ_USB_PWDN_ENABLE
#define	IRQ_USB_PWDN_ENABLE  	0
#endif
#ifndef IRQ_GPIO_ENABLE
#define IRQ_GPIO_ENABLE 		0
#endif
#ifndef IRQ_GPIO0_ENABLE
#define IRQ_GPIO0_ENABLE 		0
#endif
#if(MODULE_ETH_ENABLE)
	#define	IRQ_MAC_ENABLE		1
#else
	#define	IRQ_MAC_ENABLE		0
#endif

#ifndef IRQ_RF_RTX_ENABLE
#define IRQ_RF_RTX_ENABLE		MODULE_RF_ENABLE
#endif

#ifndef PM_CPU_SLEEP_WAIT_REQUIRED
#define PM_CPU_SLEEP_WAIT_REQUIRED		0
#endif

#ifndef BATT_ADC_CHANNEL
#define BATT_ADC_CHANNEL				0
#endif

#ifndef	DEFAULT_CUSTOM_SENSOR_OFF_IN_DEEPSLEEP
#define	DEFAULT_CUSTOM_SENSOR_OFF_IN_DEEPSLEEP		1
#endif

#ifndef	DEFAULT_CUSTOM_SENSOR_OFF_NO_RF
#define	DEFAULT_CUSTOM_SENSOR_OFF_NO_RF		0
#endif

#ifndef	DEFAULT_CUSTOM_SENSOR_ON_RF
#define	DEFAULT_CUSTOM_SENSOR_ON_RF		0
#endif

#ifndef	PM_SEARCH_TIMEOUT
#define	PM_SEARCH_TIMEOUT		4000
#endif

#ifndef	SUSPEND_SET_RX_4MS
#define	SUSPEND_SET_RX_4MS		1
#endif

/////////////////////////////////////////////////////
//	code size optimization
/////////////////////////////////////////////////////
#ifndef	CS_OPT_V_PATTERN_ON
#define	CS_OPT_V_PATTERN_ON			0
#endif

#ifndef	CS_OPT_FAST_REMOTE_WAKEUP
#define	CS_OPT_FAST_REMOTE_WAKEUP		0
#endif

#ifndef CS_OPT_TEST_MODE_LEVEL
#define	CS_OPT_TEST_MODE_LEVEL		3
#endif

#ifndef CS_OPT_LED_EN
#define	CS_OPT_LED_EN		1
#endif

#ifndef CS_OPT_BATTERY_EN
#define	CS_OPT_BATTERY_EN		0
#endif

#ifndef CS_OPT_DEEP_SLEEP_EN
#define	CS_OPT_DEEP_SLEEP_EN		1
#endif

#ifndef CS_OPT_DEEP_SLEEP_TIME_INFINITE
#define CS_OPT_DEEP_SLEEP_TIME_INFINITE	0
#endif

#ifndef DISABLE_SENSOR_OFF_POWER_ON
#define DISABLE_SENSOR_OFF_POWER_ON	0
#endif

#ifndef CS_OPT_DEVICE_INFO_EN
#define	CS_OPT_DEVICE_INFO_EN		0
#endif

#ifndef	TEST_MODE_MIDDLE_BUTTON_CLICK
#define	TEST_MODE_MIDDLE_BUTTON_CLICK	0
#endif


//////////////////    RF configuration //////////////
#ifndef RF_CHANNEL_DEBUG
#define RF_CHANNEL_DEBUG 	 	0	// using debug channel,  no hopping
#endif

#define RF_PKT_BUFF_LEN			256			// telink proprietary protocol

#define RF_RX_PKT_BUFF_COUNT	4		//  pingpong mode, use 2 buffers
#define RF_RX_USE_DMA_IRQ		0		//  pingpong mode, use 2 buffers
#define RF_TX_USE_DMA_IRQ		0		//  pingpong mode, use 2 buffers

#ifndef RF_WAIT_TOKEN_TIMEOUT

#define RF_WAIT_TOKEN_TIMEOUT			3000	//  in us,  no less than 2300
#endif


#ifndef RF_ACK_REQ_INTERVAL
#define RF_ACK_REQ_INTERVAL				200000	//  in us
#endif

#ifndef RF_IDLE_ACK_REQ_INTERVAL
#define RF_IDLE_ACK_REQ_INTERVAL		400000	//  in us
#endif

#ifndef RF_ACK_EMG_REQ_INTERVAL
#define RF_ACK_EMG_REQ_INTERVAL			20000	//  in us
#endif

#ifndef RF_CONNECTING_PKT_REQ_INTV
#define RF_CONNECTING_PKT_REQ_INTV		50000	//  in us
#endif

#ifndef RF_CONNECTING_REQ_TIMEOUT
#define RF_CONNECTING_REQ_TIMEOUT		150000	//  in us
#endif

#ifndef RF_SEARCHING_REQ_TIMEOUT
#define RF_SEARCHING_REQ_TIMEOUT		30000000	//  30second, in us
#endif

#ifndef RF_TX_IRQ_REQ_TIMEOUT
#define RF_TX_IRQ_REQ_TIMEOUT			1500	//  in us
#endif

#ifndef RF_DONGLE_RX_MODE_CYCLE
#define RF_DONGLE_RX_MODE_CYCLE			200000	//  in us
#endif

#ifndef RF_CHANNEL_IN_USB_SUSPEND
#define RF_CHANNEL_IN_USB_SUSPEND		15	//  in us
#endif

#ifndef RF_SEND_DURATION_IN_USB_SUSPEND
#define RF_SEND_DURATION_IN_USB_SUSPEND		(1000*1000)	//  in us
#endif

#ifndef RF_DELAY_SET_RX_MODE_TIME
#define RF_DELAY_SET_RX_MODE_TIME			(0)	//  in us,  must less than 500us,  cannot save much power
#endif

#ifndef MODULE_MOUSE_ENABLE
#define MODULE_MOUSE_ENABLE		0
#endif

#ifndef MOUSE_WHEEL_50K_PULLUP_EN
#define MOUSE_WHEEL_50K_PULLUP_EN	0
#endif

#ifndef RF_MAC_BROADCAST_ENABLE
#define RF_MAC_BROADCAST_ENABLE			0
#endif

/////////////////// PM  ///////////////////////////
#ifndef MOUSE_MOTION_DEEPSLEEP
#define MOUSE_MOTION_DEEPSLEEP			WAKEUP_SRC_PWM2
#endif

#ifndef PM_DEEPSLEEP_NOT_CONNECTED_WAKEUP_PIN
#define PM_DEEPSLEEP_NOT_CONNECTED_WAKEUP_PIN	0
#endif

////////////////// Keyboard //////////////////////
#ifndef KB_ONLY_SINGLEKEY_SUPP
#define KB_ONLY_SINGLEKEY_SUPP			0
#endif

///////////////////  gsensor //////////////////////////////
#ifndef GSENSOR_START_MOVING_CHECK
#define GSENSOR_START_MOVING_CHECK      0
#endif

#ifndef GSENSOR_CRANKING_ENABLE
#define GSENSOR_CRANKING_ENABLE         0
#endif

#ifndef AIRMOUSE_ENABLE_CHECK
#define AIRMOUSE_ENABLE_CHECK           0
#endif

#ifndef AIRMOUSE_ENABLE_TH
#define AIRMOUSE_ENABLE_TH              (20*1000*1000) // us
#endif

#ifndef GSENSOR_STILL_TH
#define GSENSOR_STILL_TH                (5*1000*1000) // us
#endif

#ifndef GSENSOR_CRANKING_TH
#define GSENSOR_CRANKING_TH             80 // base on MOVING_TH
#endif

////////////////// / OFN //////////////////////////////////
#ifndef GPIO_OFN_MOTION
#define GPIO_OFN_MOTION             	0 // base on MOVING_TH
#endif

#ifndef GPIO_GSENSOR
#define	GPIO_GSENSOR				0
#endif
#ifndef GPIO_GYRO
#define	GPIO_GYRO					0
#endif

#if (!__TLSR_RISCV_EN__)
///////////////////  ADC  /////////////////////////////////
// music input pin setting
enum{
	ADC_PGA_CHN_INP_NONE = 0,
	ADC_PGA_CHN_INP_LEFT_ANA1 = 1,
	ADC_PGA_CHN_INP_LEFT_ANA3 = 2,
	ADC_PGA_CHN_INP_RIGHT_ANA5 = 1,
	ADC_PGA_CHN_INP_RIGHT_ANA7 = 2,
};
// MUSIC input
#ifndef ADC_PGA_CHN0_INP_LEFT
#define ADC_PGA_CHN0_INP_LEFT		ADC_PGA_CHN_INP_NONE	// music left channel input from ana5 to adc channel
#endif
#ifndef ADC_PGA_CHN0_INP_RIGHT
#define ADC_PGA_CHN0_INP_RIGHT		ADC_PGA_CHN_INP_NONE
#endif
#ifndef ADC_PGA_CHN12_INP_LEFT
#define ADC_PGA_CHN12_INP_LEFT		ADC_PGA_CHN_INP_NONE
#endif
#ifndef ADC_PGA_CHN12_INP_RIGHT
#define ADC_PGA_CHN12_INP_RIGHT		ADC_PGA_CHN_INP_NONE
#endif

// other inputs pin setting	( 3 channels supported)
enum{
	ADC_CHN_INP_ANA_NONE = 0,
	ADC_CHN_INP_ANA_1 = 1,
	ADC_CHN_INP_ANA_2 = 2,
	ADC_CHN_INP_ANA_3 = 3,
	ADC_CHN_INP_ANA_4 = 4,
	ADC_CHN_INP_ANA_5 = 5,
	ADC_CHN_INP_ANA_6 = 6,
	ADC_CHN_INP_ANA_7 = 7,
	ADC_CHN_INP_ANA_8 = 8,
	ADC_CHN_INP_ANA_9 = 9,
	ADC_CHN_INP_ANA_10 = 10,
	ADC_CHN_INP_ANA_11 = 11,
	ADC_CHN_INP_ANA_12 = 12,
	ADC_CHN_INP_ANA_13 = 13,
	ADC_CHN_INP_ANA_14 = 14,
	ADC_CHN_INP_ANA_15 = 15,
};
// channel other than PGA/music
#ifndef ADC_CHN0_ANA_INPUT
#define ADC_CHN0_ANA_INPUT		ADC_CHN_INP_ANA_6
#endif
#ifndef ADC_CHN0_ANA_DIFF_INPUT
#define ADC_CHN0_ANA_DIFF_INPUT	ADC_CHN_INP_ANA_NONE
#endif
#ifndef ADC_CHN1_ANA_INPUT
#define ADC_CHN1_ANA_INPUT		ADC_CHN_INP_ANA_NONE
#endif
#ifndef ADC_CHN2_ANA_INPUT
#define ADC_CHN2_ANA_INPUT		ADC_CHN_INP_ANA_NONE
#endif

// adc reference voltage
enum{
	ADC_REF_SRC_INTERNAL = 0,
	ADC_REF_SRC_AVDD = 1,
	ADC_REF_SRC_EXT_AIN_10 = 2,		// exteranl analog input 10
	ADC_REF_SRC_DIFF_AIN_10_11 = 3, // differential of exteranl analog input 10 and 11
};

#ifndef ADC_CHN0_REF_SRC
#define ADC_CHN0_REF_SRC	ADC_REF_SRC_INTERNAL
#endif
#ifndef ADC_CHN1_REF_SRC
#define ADC_CHN1_REF_SRC	ADC_REF_SRC_INTERNAL
#endif
#ifndef ADC_CHN2_REF_SRC
#define ADC_CHN2_REF_SRC	ADC_REF_SRC_INTERNAL
#endif

//#include "proj/drivers/adc.h"
#ifndef ADC_CLK_CFG
#define ADC_CLK_CFG		ADC_PLL240M_6M
#endif

#ifndef ADC_CHN_SRC
#define ADC_CHN_SRC		(ADC_CHN0_SRC(ADC_SRC_INTER_AVDD_DIV3)|ADC_CHN1_SRC(ADC_SRC_INTER_AVDD_DIV3)|ADC_CHN2_SRC(ADC_SRC_INTER_AVDD_DIV3))
#endif

#ifndef ADC_CHN_REF_SRC
#define ADC_CHN_REF_SRC	(ADC_CHN0_VREF(ADC_VREF_1P3)|ADC_CHN1_VREF(ADC_VREF_1P3)|ADC_CHN2_VREF(ADC_VREF_1P3))
#endif

#ifndef ADC_MODE_CFG
#define ADC_MODE_CFG	(ADC_MODE_CHN1_EN|ADC_MODE_AOUT_EN|ADC_MODE_CHN0_EN|ADC_MODE_AUDIO_MONO)
#endif

#ifndef ADC_PERIOD_CFG
#define ADC_PERIOD_CFG 	(0x0800b8)
#endif

#ifndef ADC_REF_SELECT
#define ADC_REF_SELECT 	ADC_REF_1_3V)
#endif


///////just for compile
//  usb connected
#ifndef GPIO_USB_DET
#define	GPIO_USB_DET			0
#endif
//  battery full
#ifndef GPIO_CHARG_DET
#define	GPIO_CHARG_DET			0
#endif

///////////////////  IR  /////////////////////////////////
#ifndef IR_USE_PWM
#define IR_USE_PWM			0
#endif

#ifndef GPIO_IR
#define	GPIO_IR			    0
#endif

#ifndef IR_PWM_ID
#define IR_PWM_ID			0
#endif

#ifndef GPIO_IR_LEARN_IN
#define GPIO_IR_LEARN_IN	0
#endif

enum{
	IR_PROTOCOL_NEC,
	IR_PROTOCOL_PHILIPS
};

#ifndef IR_PROTOCOL
#define IR_PROTOCOL			IR_PROTOCOL_NEC
#endif

///////////////////  Audio  /////////////////////////////////
#if (APPLICATION_DONGLE)
#ifndef DFIFO0_SIZE
#define	DFIFO0_SIZE 		0x800
#endif
#ifndef DFIFO1_SIZE
#define	DFIFO1_SIZE 		0x200
#endif

#else
#ifndef DFIFO0_SIZE
#define	DFIFO0_SIZE 		0x400
#endif
#ifndef DFIFO1_SIZE
#define	DFIFO1_SIZE 		0x400
#endif
#endif
#define	DFIFO2_SIZE 		0x00

enum{
	MIC_SRC_FROM_USB = 0,
	MIC_SRC_FROM_I2S = 1,
	MIC_SRC_FROM_ADC = 2,
	MIC_SRC_FROM_DMIC = 3,
};

#ifndef MIC_IS_MONO
#define MIC_IS_MONO				1
#endif
#ifndef MIC_DATA_SRC
#define MIC_DATA_SRC			MIC_SRC_FROM_ADC
#endif
#ifndef MIC_FRAME_LEN
#define MIC_FRAME_LEN			192
#endif

#ifndef MIC_AUTO_CLOSE_TIMEOUT
#define MIC_AUTO_CLOSE_TIMEOUT	(3*1000*1000)	//  shutdown mic after 2s mute
#endif
#endif

///////////////////  USB   /////////////////////////////////
#ifndef USB_PRINTER_ENABLE
#define	USB_PRINTER_ENABLE 		0
#endif
#ifndef USB_SPEAKER_ENABLE
#define	USB_SPEAKER_ENABLE 		0
#endif
#ifndef USB_MIC_ENABLE
#define	USB_MIC_ENABLE 			0
#endif
#ifndef USB_MOUSE_ENABLE
#define	USB_MOUSE_ENABLE 			0
#endif
#ifndef USB_KEYBOARD_ENABLE
#define	USB_KEYBOARD_ENABLE 		0
#endif
#ifndef USB_SOMATIC_ENABLE
#define	USB_SOMATIC_ENABLE 		0
#endif
#ifndef USB_CUSTOM_HID_REPORT
#define	USB_CUSTOM_HID_REPORT 		0
#endif
#ifndef USB_AUDIO_441K_ENABLE
#define USB_AUDIO_441K_ENABLE  	0
#endif
#ifndef USB_MASS_STORAGE_ENABLE
#define USB_MASS_STORAGE_ENABLE  	0
#endif
#ifndef MIC_CHANNEL_COUNT
#define MIC_CHANNEL_COUNT  			2
#endif

#ifndef USB_DESCRIPTOR_CONFIGURATION_FOR_KM_DONGLE
#define USB_DESCRIPTOR_CONFIGURATION_FOR_KM_DONGLE  			0
#endif

#ifndef USB_ID_AND_STRING_CUSTOM
#define USB_ID_AND_STRING_CUSTOM  								0
#endif

#define KEYBOARD_RESENT_MAX_CNT			3
#define KEYBOARD_REPEAT_CHECK_TIME		300000	// in us
#define KEYBOARD_REPEAT_INTERVAL		100000	// in us
#define KEYBOARD_SCAN_INTERVAL			16000	// in us
#define MOUSE_SCAN_INTERVAL				8000	// in us
#define SOMATIC_SCAN_INTERVAL     		8000

#define USB_KEYBOARD_POLL_INTERVAL		10		// in ms	USB_KEYBOARD_POLL_INTERVAL < KEYBOARD_SCAN_INTERVAL to ensure PC no missing key
#define USB_MOUSE_POLL_INTERVAL			4		// in ms
#define USB_SOMATIC_POLL_INTERVAL     	8		// in ms

#define USB_KEYBOARD_RELEASE_TIMEOUT    (450000) // in us
#define USB_MOUSE_RELEASE_TIMEOUT       (200000) // in us
#define USB_SOMATIC_RELEASE_TIMEOUT     (200000) // in us

////////////////  ethernet /////////////////
#define	ETH_RX_PKT_BUFF_LEN			512			//  including the dam_length,  that is the max ip packet size is ETH_RX_PKT_BUFF_SIZE - 4
#define	ETH_RX_PKT_BUFF_COUNT		4 			//
#define ETH_RX_USE_DMA_IRQ			1			//  pingpong mode, use 2 buffers
#define ETH_TX_USE_DMA_IRQ			0			//  pingpong mode, use 2 buffers

////////////////  device id management  /////////////////
#if (MCU_CORE_TYPE == MCU_CORE_5330)
#define		DEVICE_ID_ADDRESS		(APPLICATION_DONGLE ? 0x1f00 : 0x1fc0)
#else
#define		DEVICE_ID_ADDRESS		0x3f00
#endif

#ifndef DID_AUTO_GEN
#define DID_AUTO_GEN			0
#endif

#ifndef DID_STORED_ADDRESS
#define DID_STORED_ADDRESS			(0x8000)
#endif

#ifndef AM_CORRECT_STORED_ADDRESS
#define AM_CORRECT_STORED_ADDRESS   (0x8800)
#endif

#ifndef	DID_PARING_MAX
#define	DID_PARING_MAX			32
#endif

#ifndef	DID_RX_ID
#define	DID_RX_ID			0x8484
#endif
///////////////////  RF4CE   /////////////////////////////////
#define TL_NEW_RF4CE		    1
// 1 means Coordinator, others mean EndDevice, for compile.
#define RF4CE_COORD				1
// 1 means beacon mode , coord start send a beacon pkt every BEACON_PKT_SEND_INTERVAL ms.
#define RF4CE_LINK_BEACON	    0

// Constant: RF4CE ZigBee Remote Control Profile ID
#define ZRC_PROFILE_ID                              0x01
// Constant: RF4CE protocol version number
#define RF4CE_VERSION_NUM                           0x01
// Constant: RF4CE protocol identifier
#define RF4CE_PROTOCOL_ID                           0xCE

#define RF4CE_RESERVED_FRAME                        0x00
#define RF4CE_STD_DATA_FRAME                        0x01
#define RF4CE_COMMAND_FRAME                         0x02
#define RF4CE_VENDOR_FRAME                          0x03

// coord addr 0x0000
#define SIMU_NWK_ADDR        0x1234
#if (RF4CE_COORD)
#define SIMU_EXT_ADDR        0x1234567890ABCDEFULL
#else
#define SIMU_EXT_ADDR        0x2345678901ABCDEFULL
#endif

#define RF4CE_ZDO_EP            0x01
#define RF4CE_RC_EP             0x02

// profile identifiers
#define RF4CE_PROFILE_ID_START                          0x01
#define RF4CE_PROFILE_CERC                              0x01
#define RF4CE_PROFILE_ID_END                            0x02

// complete application capabilities structure to send its own capabilities
#define RF4CE_MAX_NUM_DEV_TYPES                     3 // number of device types supported per device
#define RF4CE_MAX_NUM_PROFILE_IDS                   7 // number of profile IDs supported per device
#define RF4CE_VENDOR_STRING_LENGTH                  7
#define RF4CE_USER_STRING_LENGTH                    15
#define RF4CE_SEC_KEY_SEED_LENGTH                   80

#if (__TLSR_RISCV_EN__)
#if (DEBUG_GPIO_ENABLE)
	#ifdef GPIO_CHN0
		#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
		#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
		#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#else
		#define DBG_CHN0_LOW
		#define DBG_CHN0_HIGH
		#define DBG_CHN0_TOGGLE
	#endif

	#ifdef  GPIO_CHN1
		#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
		#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
		#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#else
		#define DBG_CHN1_LOW
		#define DBG_CHN1_HIGH
		#define DBG_CHN1_TOGGLE
	#endif

	#ifdef  GPIO_CHN2
		#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
		#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
		#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#else
		#define DBG_CHN2_LOW
		#define DBG_CHN2_HIGH
		#define DBG_CHN2_TOGGLE
	#endif

	#ifdef  GPIO_CHN3
		#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
		#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
		#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#else
		#define DBG_CHN3_LOW
		#define DBG_CHN3_HIGH
		#define DBG_CHN3_TOGGLE
	#endif

	#ifdef GPIO_CHN4
		#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
		#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
		#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#else
		#define DBG_CHN4_LOW
		#define DBG_CHN4_HIGH
		#define DBG_CHN4_TOGGLE
	#endif

	#ifdef  GPIO_CHN5
		#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
		#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
		#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#else
		#define DBG_CHN5_LOW
		#define DBG_CHN5_HIGH
		#define DBG_CHN5_TOGGLE
	#endif

	#ifdef  GPIO_CHN6
		#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
		#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
		#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
	#else
		#define DBG_CHN6_LOW
		#define DBG_CHN6_HIGH
		#define DBG_CHN6_TOGGLE
	#endif

	#ifdef  GPIO_CHN7
		#define DBG_CHN7_LOW		gpio_write(GPIO_CHN7, 0)
		#define DBG_CHN7_HIGH		gpio_write(GPIO_CHN7, 1)
		#define DBG_CHN7_TOGGLE		gpio_toggle(GPIO_CHN7)
	#else
		#define DBG_CHN7_LOW
		#define DBG_CHN7_HIGH
		#define DBG_CHN7_TOGGLE
	#endif

	#ifdef GPIO_CHN8
		#define DBG_CHN8_LOW		gpio_write(GPIO_CHN8, 0)
		#define DBG_CHN8_HIGH		gpio_write(GPIO_CHN8, 1)
		#define DBG_CHN8_TOGGLE		gpio_toggle(GPIO_CHN8)
	#else
		#define DBG_CHN8_LOW
		#define DBG_CHN8_HIGH
		#define DBG_CHN8_TOGGLE
	#endif

	#ifdef  GPIO_CHN9
		#define DBG_CHN9_LOW		gpio_write(GPIO_CHN9, 0)
		#define DBG_CHN9_HIGH		gpio_write(GPIO_CHN9, 1)
		#define DBG_CHN9_TOGGLE		gpio_toggle(GPIO_CHN9)
	#else
		#define DBG_CHN9_LOW
		#define DBG_CHN9_HIGH
		#define DBG_CHN9_TOGGLE
	#endif

	#ifdef  GPIO_CHN10
		#define DBG_CHN10_LOW		gpio_write(GPIO_CHN10, 0)
		#define DBG_CHN10_HIGH		gpio_write(GPIO_CHN10, 1)
		#define DBG_CHN10_TOGGLE	gpio_toggle(GPIO_CHN10)
	#else
		#define DBG_CHN10_LOW
		#define DBG_CHN10_HIGH
		#define DBG_CHN10_TOGGLE
	#endif

	#ifdef  GPIO_CHN11
		#define DBG_CHN11_LOW		gpio_write(GPIO_CHN11, 0)
		#define DBG_CHN11_HIGH		gpio_write(GPIO_CHN11, 1)
		#define DBG_CHN11_TOGGLE	gpio_toggle(GPIO_CHN11)
	#else
		#define DBG_CHN11_LOW
		#define DBG_CHN11_HIGH
		#define DBG_CHN11_TOGGLE
	#endif

	#ifdef GPIO_CHN12
		#define DBG_CHN12_LOW		gpio_write(GPIO_CHN12, 0)
		#define DBG_CHN12_HIGH		gpio_write(GPIO_CHN12, 1)
		#define DBG_CHN12_TOGGLE	gpio_toggle(GPIO_CHN12)
	#else
		#define DBG_CHN12_LOW
		#define DBG_CHN12_HIGH
		#define DBG_CHN12_TOGGLE
	#endif

	#ifdef  GPIO_CHN13
		#define DBG_CHN13_LOW		gpio_write(GPIO_CHN13, 0)
		#define DBG_CHN13_HIGH		gpio_write(GPIO_CHN13, 1)
		#define DBG_CHN13_TOGGLE	gpio_toggle(GPIO_CHN13)
	#else
		#define DBG_CHN13_LOW
		#define DBG_CHN13_HIGH
		#define DBG_CHN13_TOGGLE
	#endif

	#ifdef  GPIO_CHN14
		#define DBG_CHN14_LOW		gpio_write(GPIO_CHN14, 0)
		#define DBG_CHN14_HIGH		gpio_write(GPIO_CHN14, 1)
		#define DBG_CHN14_TOGGLE	gpio_toggle(GPIO_CHN14)
	#else
		#define DBG_CHN14_LOW
		#define DBG_CHN14_HIGH
		#define DBG_CHN14_TOGGLE
	#endif

	#ifdef  GPIO_CHN15
		#define DBG_CHN15_LOW		gpio_write(GPIO_CHN15, 0)
		#define DBG_CHN15_HIGH		gpio_write(GPIO_CHN15, 1)
		#define DBG_CHN15_TOGGLE	gpio_toggle(GPIO_CHN15)
	#else
		#define DBG_CHN15_LOW
		#define DBG_CHN15_HIGH
		#define DBG_CHN15_TOGGLE
	#endif
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
	#define DBG_CHN8_LOW
	#define DBG_CHN8_HIGH
	#define DBG_CHN8_TOGGLE
	#define DBG_CHN9_LOW
	#define DBG_CHN9_HIGH
	#define DBG_CHN9_TOGGLE
	#define DBG_CHN10_LOW
	#define DBG_CHN10_HIGH
	#define DBG_CHN10_TOGGLE
	#define DBG_CHN11_LOW
	#define DBG_CHN11_HIGH
	#define DBG_CHN11_TOGGLE
	#define DBG_CHN12_LOW
	#define DBG_CHN12_HIGH
	#define DBG_CHN12_TOGGLE
	#define DBG_CHN13_LOW
	#define DBG_CHN13_HIGH
	#define DBG_CHN13_TOGGLE
	#define DBG_CHN14_LOW
	#define DBG_CHN14_HIGH
	#define DBG_CHN14_TOGGLE
	#define DBG_CHN15_LOW
	#define DBG_CHN15_HIGH
	#define DBG_CHN15_TOGGLE
#endif
#endif

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
