/********************************************************************************************************
 * @file	adc.c
 *
 * @brief	This is the ADC driver file for TLSR8258
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
#include "adc.h"
#include "analog.h"
#include "clock.h"
#include "dfifo.h"
#include "timer.h"
#include "proj_lib/sig_mesh/app_mesh.h"
_attribute_data_retention_
adc_vref_ctr_t adc_vref_cfg = {
	.adc_vref 		= 1175, //default ADC ref voltage (unit:mV)
	.adc_calib_en	= 1, 	//default enable
};

volatile unsigned short	adc_code;
unsigned char   adc_pre_scale;
unsigned short adc_ref_vol[2] 	= {600,900};
GPIO_PinTypeDef ADC_GPIO_tab[10] = {
		GPIO_PB0,GPIO_PB1,
		GPIO_PB2,GPIO_PB3,
		GPIO_PB4,GPIO_PB5,
		GPIO_PB6,GPIO_PB7,
		GPIO_PC4,GPIO_PC5
};

/**
 * @brief This function is used for IO port configuration of ADC supply voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_pin_init(GPIO_PinTypeDef pin)
{
	//ADC GPIO Init
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,0);
	gpio_write(pin,0);
}

/**
 * @brief This function is used for IO port configuration of ADC supply voltage sampling.
 * @param[in]  pin -  GPIO_PinTypeDef pin
 * @return none
 */
void adc_vbat_pin_init(GPIO_PinTypeDef pin)
{
	gpio_set_func(pin, AS_GPIO);
	gpio_set_input_en(pin,0);
	gpio_set_output_en(pin,1);
	gpio_write(pin,1);
}

/**
 * @brief This function serves to set the channel reference voltage.
 * @param[in]   ch_n - enum variable of ADC input channel.
 * @param[in]   v_ref - enum variable of ADC reference voltage.
 * @return none
 */
void adc_set_ref_voltage(ADC_ChTypeDef ch_n, ADC_RefVolTypeDef v_ref)
{
	//any two channel can not be used at the same time
	adc_set_vref(v_ref, v_ref, v_ref);
	if(v_ref == ADC_VREF_1P2V)
	{
		//Vref buffer bias current trimming: 		150%
		//Comparator preamp bias current trimming:  100%
		analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x3d );
	}
	else
	{
		//Vref buffer bias current trimming: 		100%
		//Comparator preamp bias current trimming:  100%
		analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(0xC0)) | 0x15 );
		adc_vref_cfg.adc_vref=adc_ref_vol[v_ref];
	}


}

/**
 * @brief This function serves to set resolution.
 * @param[in]   ch_n - enum variable of ADC input channel.
 * @param[in]   v_res - enum variable of ADC resolution.
 * @return none
 */
void adc_set_resolution(ADC_ChTypeDef ch_n, ADC_ResTypeDef v_res)

{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_resolution_chn_left(v_res);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_resolution_chn_right(v_res);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_resolution_chn_misc(v_res);
	}
}

/**
 * @brief This function serves to set sample_cycle.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  adcST - enum variable of ADC Sampling cycles.
 * @return none
 */
void adc_set_tsample_cycle(ADC_ChTypeDef ch_n, ADC_SampCycTypeDef adcST)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_tsample_cycle_chn_left(adcST);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_tsample_cycle_chn_right(adcST);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_tsample_cycle_chn_misc(adcST);
	}
}

/**
 * @brief This function serves to set input_mode.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  m_input - enum variable of ADC channel input mode.
 * @return none
 */
void adc_set_input_mode(ADC_ChTypeDef ch_n,  ADC_InputModeTypeDef m_input)

{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_input_mode_chn_left(m_input);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_input_mode_chn_right(m_input);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_input_mode_chn_misc(m_input);
	}
}

/**
 * @brief This function serves to set input channel in single_ended_input_mode.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  InPCH - enum variable of ADC analog positive input channel.
 * @return none
 */
void adc_set_ain_channel_single_ended_input_mode(ADC_ChTypeDef ch_n, ADC_InputPchTypeDef InPCH)
{
	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_ain_negative_chn_left(GND);
		adc_set_ain_positive_chn_left(InPCH);
		adc_set_input_mode_chn_left(SINGLE_ENDED_MODE);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_ain_negative_chn_right(GND);
		adc_set_ain_positive_chn_right(InPCH);
		adc_set_input_mode_chn_right(SINGLE_ENDED_MODE);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_ain_negative_chn_misc(GND);
		adc_set_ain_positive_chn_misc(InPCH);
		adc_set_input_mode_chn_misc(SINGLE_ENDED_MODE);
	}
}

/**
 * @brief This function serves to set input channel in differential_mode.
 * @param[in]  ch_n - enum variable of ADC input channel.
 * @param[in]  InPCH - enum variable of ADC analog positive input channel.
 * @param[in]  InNCH - enum variable of ADC analog negative input channel.
 * @return none
 */
void adc_set_ain_channel_differential_mode(ADC_ChTypeDef ch_n, ADC_InputPchTypeDef InPCH,ADC_InputNchTypeDef InNCH)
{

	if(ch_n & ADC_LEFT_CHN)
	{
		adc_set_ain_chn_left(InPCH, InNCH);
		adc_set_input_mode_chn_left(DIFFERENTIAL_MODE);
	}
	if(ch_n & ADC_RIGHT_CHN)
	{
		adc_set_ain_chn_right(InPCH, InNCH);
		adc_set_input_mode_chn_right(DIFFERENTIAL_MODE);
	}
	if(ch_n & ADC_MISC_CHN)
	{
		adc_set_ain_chn_misc(InPCH, InNCH);
		adc_set_input_mode_chn_misc(DIFFERENTIAL_MODE);
	}
}


/**
 * @brief This function serves to set pre_scaling.
 * @param[in]  v_scl - enum variable of ADC pre_scaling factor.
 * noticew: hen adc reference voltage is ADC_VREF_0P6V/ADC_VREF_0P9V, pre_scaling factor(v_scl ) must be ADC_PRESCALER_1.
 * @return none
 */
void adc_set_ain_pre_scaler(ADC_PreScalingTypeDef v_scl)
{
	analog_write( areg_ain_scale  , (analog_read( areg_ain_scale  )&(~FLD_SEL_AIN_SCALE)) | (v_scl<<6) );
	//setting adc_sel_atb ,if stat is 0,clear adc_sel_atb,else set adc_sel_atb[0]if(stat)
	unsigned char tmp;
	if(v_scl)
	{
		//ana_F9<4> must be 1
		tmp = analog_read(0xF9);
		tmp = tmp|0x10;                    //open tmp = tmp|0x10;
		analog_write (0xF9, tmp);
	}
	else
	{
		//ana_F9 <4> <5> must be 0
		tmp = analog_read(0xF9);
		tmp = tmp&0xcf;
		analog_write (0xF9, tmp);
	}
	adc_pre_scale = 1<<(unsigned char)v_scl;
}

/**
 * @brief This function serves to ADC init.
 * @param[in]   none
 * @return none
 */
void adc_init(void){
	/******power off sar adc********/
	adc_power_on_sar_adc(0);
	/****** sar adc Reset ********/
	//reset whole digital adc module
	adc_reset_adc_module();
	/******enable signal of 24M clock to sar adc********/
	adc_enable_clk_24m_to_sar_adc(1);
	/******set adc clk as 4MHz******/
	adc_set_sample_clk(5);

	pga_left_chn_power_on(0);
	pga_right_chn_power_on(0);

	/******set adc L R channel Gain Stage bias current trimming******/
	adc_set_left_gain_bias(GAIN_STAGE_BIAS_PER100);
	adc_set_right_gain_bias(GAIN_STAGE_BIAS_PER100);
	dfifo_disable_dfifo2();//disable misc channel data dfifo
}

/**
 * @brief This function is used for ADC configuration of ADC IO voltage sampling.
 * @param[in]   pin - GPIO_PinTypeDef
 * @return none
 */
void adc_base_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_num=0;
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);//enable the mic channel and set max_state_cnt

if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_23K)
{

	//This configure from Jianbo and Congqing. The purpose of this configuration is to prevent leakage
	adc_set_state_length(1023, 0, 15);  	//set R_max_mc=240,R_max_s=15
//	analog_write(areg_r_max_s, (analog_read(areg_r_max_s) & 0x3f) | 0xc0 );
}
else if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_96K)
{
	adc_set_state_length(240, 0, 10);  	//set R_max_mc=240,R_max_s=10
}

	adc_set_ref_voltage(ADC_MISC_CHN, ADC_VREF_1P2V);//set channel Vref,
   // ADC_Vref = (unsigned char)ADC_VREF_1P2V;
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);//set Vbat divider select,
//	ADC_VBAT_Scale = VBAT_Scale_tab[ADC_VBAT_DIVIDER_OFF];

	adc_base_pin_init(pin);		//ADC GPIO Init
	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_num = i+1;
			break;
		}
	}
	adc_set_ain_channel_differential_mode(ADC_MISC_CHN, gpio_num, GND);
	adc_set_resolution(ADC_MISC_CHN, RES14);
	adc_set_tsample_cycle(ADC_MISC_CHN, SAMPLING_CYCLES_6);
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);//adc scaling factor is 1/8
	adc_set_mode(ADC_NORMAL_MODE);//set RORMAl mode


}


/**
 * @brief This function is used for ADC configuration of ADC supply voltage sampling.
 * @param[in]    pin - GPIO_PinTypeDef
 * @return none
 */
void adc_vbat_init(GPIO_PinTypeDef pin)
{
	unsigned char i;
	unsigned char gpio_no=0;
	adc_set_chn_enable_and_max_state_cnt(ADC_MISC_CHN, 2);
	adc_set_state_length(240, 0, 10);  	//set R_max_mc,R_max_c,R_max_s

	//set Vbat divider select,
	adc_set_vref_vbat_divider(ADC_VBAT_DIVIDER_OFF);
	//ADC_VBAT_Scale = VBAT_Scale_tab[ADC_VBAT_DIVIDER_OFF];
	//set channel mode and channel
	adc_vbat_pin_init(pin);
	for(i=0;i<10;i++)
	{
		if(pin == ADC_GPIO_tab[i])
		{
			gpio_no = i+1;
			break;
		}
	}
	adc_set_input_mode(ADC_MISC_CHN, DIFFERENTIAL_MODE);
	adc_set_ain_channel_differential_mode(ADC_MISC_CHN, gpio_no, GND);
	adc_set_ref_voltage(ADC_MISC_CHN, ADC_VREF_1P2V);//set channel Vref ,must be ADC_VREF_1P2V
	//ADC_Vref = (unsigned char)ADC_VREF_1P2V;
	adc_set_resolution(ADC_MISC_CHN, RES14);//set resolution
	//Number of ADC clock cycles in sampling phase
	adc_set_tsample_cycle(ADC_MISC_CHN, SAMPLING_CYCLES_6);
	//set Analog input pre-scaling and
	adc_set_ain_pre_scaler(ADC_PRESCALER_1F8);//only support ADC_PRESCALER_1F4 or  ADC_PRESCALER_1F8.
	//set NORMAL mode
	adc_set_mode(ADC_NORMAL_MODE);

}



#define ADC_SAMPLE_NUM		8 //8 /4

/**
 * @brief This function serves to set adc sampling and get results.
 * @param[in]  none.
 * @return the result of sampling.
 */
unsigned int adc_sample_and_get_result(void)
{
	unsigned short temp;
	volatile signed int adc_data_buf[ADC_SAMPLE_NUM];  //size must 16 byte aligned(16/32/64...)

	int i,j;
	unsigned int  adc_vol_mv = 0;
	unsigned short adc_sample[ADC_SAMPLE_NUM] = {0};
	unsigned short  adc_result=0;

	adc_reset_adc_module();
	unsigned int t0 = clock_time();

	for(i=0;i<ADC_SAMPLE_NUM;i++){   	//dfifo data clear
		adc_data_buf[i] = 0;
	}

	if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_23K)
	{
		while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
	}
	else if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_96K)
	{
		while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
	}

	//dfifo setting will lose in suspend/deep, so we need config it every time
	adc_config_misc_channel_buf((unsigned short *)adc_data_buf, ADC_SAMPLE_NUM<<2);  //size: ADC_SAMPLE_NUM*4
	dfifo_enable_dfifo2();
//////////////// get adc sample data and sort these data ////////////////
	for(i=0;i<ADC_SAMPLE_NUM;i++){
		/*wait for new adc sample data, When the data is not zero and more than 1.5 sampling times (when the data is zero),
	 The default data is already ready.*/


		if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_23K)
		{
			while(!clock_time_exceed(t0, 90));  //wait at least 2 sample cycle(f = 23K, T = 43.4us)
		}
		else if(ADC_SAMPLE_RATE_SELECT==ADC_SAMPLE_RATE_96K)
		{
			while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)
		}

		t0 = clock_time();
		if(adc_data_buf[i] & BIT(13)){  //14 bit resolution, BIT(13) is sign bit, 1 means negative voltage in differential_mode
			adc_sample[i] = 0;
		}
		else{
			adc_sample[i] = ((unsigned short)adc_data_buf[i] & 0x1FFF);  //BIT(12..0) is valid adc result
		}

		//insert sort
		if(i){
			if(adc_sample[i] < adc_sample[i-1]){
				temp = adc_sample[i];
				adc_sample[i] = adc_sample[i-1];
				for(j=i-1;j>=0 && adc_sample[j] > temp;j--){
					adc_sample[j+1] = adc_sample[j];
				}
				adc_sample[j+1] = temp;
			}
		}
	}


//////////////////////////////////////////////////////////////////////////////
	dfifo_disable_dfifo2();   //misc channel data dfifo disable
///// get average value from raw data(abandon some small and big data ), then filter with history data //////
#if (ADC_SAMPLE_NUM == 4)  	//use middle 2 data (index: 1,2)
	unsigned int adc_average = (adc_sample[1] + adc_sample[2])/2;
#elif(ADC_SAMPLE_NUM == 8) 	//use middle 4 data (index: 2,3,4,5)
	unsigned int adc_average = (adc_sample[2] + adc_sample[3] + adc_sample[4] + adc_sample[5])/4;
#endif
	adc_code=adc_result = adc_average;
	LOG_USER_MSG_INFO(0, 0,"ADC SAMPLE DATA:%u",adc_result);

	 //////////////// adc sample data convert to voltage(mv) ////////////////
	//                          (Vref, adc_pre_scale)   (BIT<12~0> valid data)
	//			 =  adc_result * Vref * adc_pre_scale / 0x2000
	//           =  adc_result * adc_pre_scale*Vref >>13

	adc_vol_mv  = (adc_result * adc_pre_scale*adc_vref_cfg.adc_vref)>>13;



	return adc_vol_mv;
}

/**
 * @brief This function serves to adc module setting init.
 * @param[in]  none.
 * @return none.
 */
#if ADC_ENABLE
void adc_drv_init(){	// take about 750us in 16MHz clock.
	adc_init();
	#if(ADC_MODE==ADC_BASE_MODE)
	adc_base_init(ADC_CHNM_ANA_INPUT);	
	#elif (ADC_MODE==ADC_VBAT_MODE)
	adc_vbat_init(ADC_CHNM_ANA_INPUT);
	#endif
	adc_set_ain_pre_scaler(ADC_PRESCALER);//ADC pre_scaling default value is ADC_PRESCALER_1F8, it can change after adc_base_init().
	adc_power_on_sar_adc(1);		//After setting the ADC parameters, turn on the ADC power supply control bit
	#if (BATT_CHECK_ENABLE)
	adc_hw_initialized = 0;	// inform battery check to re-init.
	#endif
}
#endif



















