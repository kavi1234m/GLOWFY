/********************************************************************************************************
 * @file	i2c_msc.c
 *
 * @brief	This is the source file for TLSR8258
 *
 * @author	Driver Group
 * @date	May 8, 2018
 *
 * @par     Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "mi_config.h"
#if HAVE_MSC
#include "drivers/8258/clock.h"
#include "drivers/8258/gpio.h"
#include "drivers/8258/timer.h"
#include "mible_type.h"
#include "i2c_msc.h"

/**
 *  @brief      This function serves to write a packet of data to the specified address of slave device
 *  @param[in]  Addr - the register that master write data to slave in. support one byte and two bytes. i.e param2 AddrLen may be 1 or 2.
 *  @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3. based on the spec of i2c slave.
 *  @param[in]  dataBuf - the first SRAM buffer address to write data to slave in.
 *  @param[in]  dataLen - the length of data master write to slave.
 *  @return     none
 */

 unsigned char i2c_write_series_for_msc(unsigned char Slave_addr,unsigned int W_Addr, unsigned int W_AddrLen, unsigned char * dataBuf, int dataLen,unsigned char iic_stop)
{
	unsigned int r = irq_disable();
    unsigned char status = IIC_EVT_XFER_DONE;
    
	reg_i2c_id	 = (Slave_addr<<1)&(~FLD_I2C_WRITE_READ_BIT); //SlaveID & 0xfe,.i.e write data. R:High  W:Low
    //launch start /id    start
    reg_i2c_ctrl = (FLD_I2C_CMD_START );// when addr=0,other (not telink) iic device no need start signal
	sleep_us(50);
	reg_i2c_ctrl =  FLD_I2C_CMD_ID;
    while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
    if(reg_i2c_status & FLD_I2C_NAK){
        status = IIC_EVT_ADDRESS_NACK;

            //stop
        reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
        while(reg_i2c_status & FLD_I2C_CMD_BUSY );

        return status;
    }

	//write data
	unsigned int buff_index = 0;
	for(buff_index=0;buff_index<dataLen;buff_index++){
		reg_i2c_di = dataBuf[buff_index];
		reg_i2c_ctrl = FLD_I2C_CMD_DI; //launch data read cycle
		while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
        if(reg_i2c_status & FLD_I2C_NAK){
            status = IIC_EVT_DATA_NACK;
                                //stop
            reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
            while(reg_i2c_status & FLD_I2C_CMD_BUSY );

            return status;
        }      
	}
    if(iic_stop == 0){
        //stop
        reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
        while(reg_i2c_status & FLD_I2C_CMD_BUSY );

    }
	irq_restore(r);
    return status;
}


/**
 * @brief      This function serves to read a packet of data from the specified address of slave device
 * @param[in]  Addr - the register master read data from slave in. support one byte and two bytes.
 * @param[in]  AddrLen - the length of register. enum 0 or 1 or 2 or 3 based on the spec of i2c slave.
 * @param[in]  dataBuf - the first address of SRAM buffer master store data in.
 * @param[in]  dataLen - the length of data master read from slave.
 * @return     none.
 */
unsigned char i2c_read_series_msc(unsigned char Slave_addr,unsigned int R_Addr, unsigned int R_AddrLen, unsigned char * dataBuf, int dataLen)
{
	unsigned int r = irq_disable();
    unsigned char status = IIC_EVT_XFER_DONE;

	//start + id(Read)
	reg_i2c_id	 = (Slave_addr<<1)|(FLD_I2C_WRITE_READ_BIT);  //SlaveID & 0xfe,.i.e write data. R:High  W:Low
	reg_i2c_ctrl = (FLD_I2C_CMD_START);
	sleep_us(50);
	reg_i2c_ctrl = FLD_I2C_CMD_ID;
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
    if(reg_i2c_status & FLD_I2C_NAK){
        status = IIC_EVT_ADDRESS_NACK;

    	//terminate
    	reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
    	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);

        return status;
    
    }

	//read data
	unsigned int bufIndex = 0;

	dataLen--;    //the length of reading data must larger than 0
	//if not the last byte master read slave, master wACK to slave
	while(dataLen){  //    
//		reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID| FLD_I2C_CMD_ACK);
        reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID);
		while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
		dataBuf[bufIndex] = reg_i2c_di;           
		bufIndex++;
		dataLen--;
	}
	//when the last byte, master will ACK to slave
	reg_i2c_ctrl = (FLD_I2C_CMD_DI | FLD_I2C_CMD_READ_ID | FLD_I2C_CMD_ACK);
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
	dataBuf[bufIndex] = reg_i2c_di;

	//terminate
	reg_i2c_ctrl = FLD_I2C_CMD_STOP; //launch stop cycle
	while(reg_i2c_status & FLD_I2C_CMD_BUSY	);
	irq_restore(r);
    return status;
}
#endif
