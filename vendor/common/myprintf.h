/********************************************************************************************************
 * @file	myprintf.h
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
#ifndef MYPRINTF_H
#define MYPRINTF_H

#if __TLSR_RISCV_EN__
#include "stack/ble/ble.h"
#else
#include "proj_lib/ble/service/ble_ll_ota.h"
#endif

#define SIMU_BAUD_115200    115200
#define SIMU_BAUD_230400    230400
#define SIMU_BAUD_1M        1000000

#define BAUD_USE    SIMU_BAUD_115200
#if __TLSR_RISCV_EN__
#define SIMU_UART_IRQ_EN    (1&&!blotaSvr.ota_start_tick)
#else
#define SIMU_UART_IRQ_EN    (1&&!blcOta.ota_start_flag)
#endif

#define _PRINT_FUN_RAMCODE_        //_attribute_ram_code_

extern  void uart_simu_send_bytes(u8 *p,int len);
//#define debugBuffer (*(volatile unsigned char (*)[40])(0x8095d8))

extern volatile u32 log_one_byte_test_flag;
void uart_put_char_one_byte_test(u8 byte, int enter_flag);
#endif
