/********************************************************************************************************
 * @file	main.c
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
#include "vendor/GLOWFY/APPLICATION/application.h"
#include "vendor/GLOWFY/HAL/hal_peripheral_init.h"
#include "proj/mcu/compiler.h"
#include "vendor/GLOWFY/MIDDLEWARE/button.h"


_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler ();  //ble irq proc

	if((reg_irq_src & FLD_IRQ_GPIO_EN)==FLD_IRQ_GPIO_EN)
	{
		reg_irq_src = FLD_IRQ_GPIO_EN;
		LOG_USER_MSG_INFO(0, 0,"interrupt");
		button_read();
	}

}

FLASH_ADDRESS_DEFINE

_attribute_ram_code_ int main (void)    //must run in ramcode
{
    system_init();

    hal_peripheral_init();

    appMain_init();
}

