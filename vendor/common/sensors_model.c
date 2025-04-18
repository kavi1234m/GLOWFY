/********************************************************************************************************
 * @file	sensors_model.c
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
#include "tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "mesh_node.h"
#include "sensors_model.h"

u8 sensor_get = 0;
#if(MD_SENSOR_EN || MD_BATTERY_EN || MD_LOCATION_EN ||BATTERY_SERVER_LOCATE_EXCLUSIVE_ELEMENT_EN)
u32 mesh_md_sensor_addr = FLASH_ADR_MD_SENSOR;
model_sensor_t			model_sig_sensor;
#endif

#if(MD_SENSOR_EN)
#if MD_SENSOR_SERVER_EN
#if !WIN32
STATIC_ASSERT(MD_LOCATION_EN == 0);// because use same flash sector to save in mesh_save_map, and should be care of OTA new firmware which add MD_BATTERY_EN
#endif
//STATIC_ASSERT(SENSOR_NUMS == 1);
STATIC_ASSERT(SENSOR_DATA_RAW_MAX_LEN <= SIZEOF_MEMBER(cadence_unit_t, cadence_low)); // delta and cadence in cadence_unit_t is u32 now

u32 sensor_measure_ms = 0;
u32 sensor_measure_quantity = 0;

// Sensor Data and Sensor Series Column
u8 sensor_data_raw0[SENSOR_DATA_RAW0_LEN];
#if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
u8 sensor_data_raw1[SENSOR_DATA_RAW1_LEN];
#endif

sensor_series_col_t sensor_series_col1;
#if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
sensor_series_col_t sensor_series_col2;
#endif

sensor_data_t sensor_data[SENSOR_NUMS] = {
	{SENSOR_PROP_ID, sizeof(sensor_data_raw0), sensor_data_raw0, &sensor_series_col1},
    #if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
	{SENSOR_SETTING_PROP_ID1, sizeof(sensor_data_raw1), sensor_data_raw1, &sensor_series_col2},//
   #endif
};

// Sensor Descriptor
#if ZSIR1000_AMBIENT_LIGHT_SENSED_EN
#define SENSOR_SAMPLE_INTERVAL			0x0a // unit: (2^n) ms
#elif ZSIR1000_MOTION_SENSED_EN
#define SENSOR_SAMPLE_INTERVAL			0x07
#else
#define SENSOR_SAMPLE_INTERVAL			0x05
#endif

const mesh_cmd_sensor_descript_st_t sensor_descrip[SENSOR_NUMS] = { 
    {SENSOR_PROP_ID, 0x347, 0x256, 0x02, SENSOR_SAMPLE_INTERVAL, 0x4B},
    #if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
    {SENSOR_SETTING_PROP_ID1, 0x347, 0x256, 0x02, SENSOR_SAMPLE_INTERVAL, 0x4B},//
    #endif
};

// Sensor Setting Map in model_sig_sensor
const sensor_setting_tbl_t sensor_settings[SENSOR_NUMS][SENSOR_SETTINGS_NUMS] = {

{
	{SENSOR_SETTING_PROP_ID0, READ_WRITE, SENSOR_SETTING_RAW0_LEN, model_sig_sensor.sensor_states[0].setting.setting_raw},
#if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
	{SENSOR_SETTING_PROP_ID1, READ_WRITE, SENSOR_SETTING_RAW1_LEN, model_sig_sensor.sensor_states[1].setting.setting_raw  /*+ SENSOR_SETTING_RAW0_LEN*/},//
#endif
},

};

sensor_setting_tbl_t *get_sensor_setting_tbl(u16 sensor_idx, u16 setting_id)
{
	sensor_setting_tbl_t *p_setting_tbl = 0;
	if(sensor_idx < SENSOR_NUMS){
		foreach(i, SENSOR_SETTINGS_NUMS){
			p_setting_tbl = (sensor_setting_tbl_t *)&sensor_settings[sensor_idx][i];
			if(setting_id == p_setting_tbl->setting_id){
				return p_setting_tbl;
			}
		}
	}

	return 0;
}

u32 get_prop_id_index(u16 prop_id)
{
	foreach(i, SENSOR_NUMS){
		if(prop_id == model_sig_sensor.sensor_states[i].prop_id){
			return i;
		}
	}
	
	return -1;
}

sensor_data_t *get_sensor_data(u16 prop_id)
{
 	sensor_data_t *p_sensor_data = 0;
	foreach(i, SENSOR_NUMS){
		p_sensor_data = &sensor_data[i];
		if(prop_id == p_sensor_data->prop_id){
			return p_sensor_data;
		}
	}

	return 0;
}

void mesh_global_var_init_sensor_descrip()
{	
	foreach_arr(i,sensor_descrip){	
		model_sig_sensor.sensor_states[i].prop_id = sensor_descrip[i].prop_id;
		model_sig_sensor.sensor_states[i].cadence.fast_period_div = FAST_CADENCE_PERIOD_DIV;
		model_sig_sensor.sensor_states[i].cadence.trig_type = TRIGGER_TYPE_DEFAULT;
        model_sig_sensor.sensor_states[i].cadence.cadence_unit.delta_down = TRIGGER_DELTA_DOWN_DEFAULT;
        model_sig_sensor.sensor_states[i].cadence.cadence_unit.delta_up = TRIGGER_DELTA_UP_DEFAULT;
        model_sig_sensor.sensor_states[i].cadence.cadence_unit.min_interval = CADENCE_MIN_INTERVAL_DEFAULT;
        model_sig_sensor.sensor_states[i].cadence.cadence_unit.cadence_low = FAST_CADENCE_LOW_DEFAULT;
        model_sig_sensor.sensor_states[i].cadence.cadence_unit.cadence_high = FAST_CADENCE_HIGH_DEFAULT;
		for(u8 j=0; j<SENSOR_SETTINGS_NUMS; j++){
			model_sig_sensor.sensor_states[i].setting.setting_id[j] = sensor_settings[i][j].setting_id;
			model_sig_sensor.sensor_states[i].setting.setting_access[j] = sensor_settings[i][j].setting_access;
			// init setting raw below
		}
	}
}

u32 mesh_sensor_possible_positive_error()
{
	return 100*sensor_descrip[0].positive_tolerance/4095;
}

u32 mesh_sensor_possible_negative_error()
{
	return 100*sensor_descrip[0].negative_tolerance/4095;
}

int mesh_cmd_sig_sensor_descript_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	int err = 0;
	
	model_g_light_s_t *p_model = (model_g_light_s_t *)cb_par->model;
	mesh_cmd_sensor_descript_st_t rsp[SENSOR_NUMS];
	u32 len = sizeof(rsp);
	u16 prop_id = par[0] + (par[1]<<8);

	if(PROHIBITED_PROP_ID == prop_id){
		return -1;
	}
	else if(par_len >= 2){ // prop id exist
		len = sizeof(mesh_cmd_sensor_descript_st_t);
		rsp[0].prop_id = prop_id;
		u32 index = get_prop_id_index(prop_id);
		
		if(index < SENSOR_NUMS){ 
			memcpy(rsp, &sensor_descrip[index], len);
		}
		else{
			len = 2;
		}
	}
	else{
		memcpy(rsp, sensor_descrip, sizeof(rsp));
	}
	
	if(cb_par->op_rsp != STATUS_NONE){
		err = mesh_tx_cmd_rsp(cb_par->op_rsp, (u8 *)&rsp, len, p_model->com.ele_adr, cb_par->adr_src, 0, 0);
	}
	
	return err;
}


int mesh_tx_sensor_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id, int id_exist)
{
	if(id_exist && (PROHIBITED_PROP_ID == prop_id)){
		return -1;
	}

	sensor_mpid_A_t rsp[SENSOR_NUMS];
	sensor_data_t *p_sensor_data = 0;
	u8 len = 0;
	
	rsp[0].format = SENSOR_DATA_FORMAT_A;
	rsp[0].prop_id = prop_id;

	if(id_exist){
		p_sensor_data = get_sensor_data(prop_id);
	}

	if(p_sensor_data){
		rsp[0].length = p_sensor_data->len_raw - 1; // The range 0x0-0x7E represents the values in the range 1-127.
		len = sizeof(sensor_mpid_A_t);
		memcpy(rsp[0].raw_value, p_sensor_data->p_raw, p_sensor_data->len_raw);
	}
	else if(id_exist){

		len = OFFSETOF(sensor_mpid_A_t,raw_value);
		rsp[0].length = -1;
	}
	else{// get all property value
		for(u8 i=0; i<SENSOR_NUMS; i++)
		{
//			LOG_USER_MSG_INFO(0, 0, "publish sensor status");
			p_sensor_data = &sensor_data[i];
			rsp[i].format = SENSOR_DATA_FORMAT_A;
			rsp[i].length = p_sensor_data->len_raw - 1;	// The range 0x0-0x7E represents the values in the range 1-127.
			rsp[i].prop_id = p_sensor_data->prop_id;
			memcpy(rsp[i].raw_value, p_sensor_data->p_raw, p_sensor_data->len_raw);
            if(!sensor_get)
            {
            	len = sizeof(rsp[0]);
    			if(i == 0)
    			{
    				mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp[i], len, ele_adr, dst_adr, uuid, pub_md);

    			}
    			else
    			{
    				mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp[i], len, ele_adr, dst_adr, uuid, pub_md);
    			}
            }


		}
	}
	if(sensor_get)
	{
		sensor_get = 0;
		len = sizeof(rsp);
		mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
	}
    sensor_measure_ms = clock_time_ms();
	return 0;
}

int mesh_sensor_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id, int id_exist)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_sensor_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0, cb_par->op_rsp, prop_id, id_exist);
}

int mesh_cmd_sig_sensor_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	u16 prop_id = 0;
	if(par_len){
		prop_id = par[0] + (par[1]<<8);
	}
	sensor_get =1;
	return mesh_sensor_st_rsp(cb_par, prop_id, par_len);
}

int mesh_tx_cadence_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id)
{
	if(PROHIBITED_PROP_ID == prop_id){
		return -1;
	}
	
	static sensor_cadence_st_t rsp;
	u8 len = OFFSETOF(sensor_cadence_st_t, cadence);
	sensor_data_t *p_sensor_data = get_sensor_data(prop_id);
	u32 id_index = get_prop_id_index(prop_id);
	rsp.prop_id = prop_id;

	if(p_sensor_data){
		u8 delta_len = p_sensor_data->len_raw;
		u8 cadence_len = delta_len;
		sensor_cadence_t *p_cadence = (sensor_cadence_t *)&model_sig_sensor.sensor_states[id_index].cadence;
		rsp.cadence.trig_type = p_cadence->trig_type;
		rsp.cadence.fast_period_div = p_cadence->fast_period_div;
		
		if(p_cadence->trig_type){
			delta_len = SIZEOF_MEMBER(cadence_unitless_t, delta_down);
		}
		
		memcpy(rsp.cadence.par, &p_cadence->cadence_unit.delta_down, delta_len);
		memcpy(rsp.cadence.par+delta_len, &p_cadence->cadence_unit.delta_up, delta_len);
		rsp.cadence.par[2*delta_len] = p_cadence->cadence_unit.min_interval;
		memcpy(rsp.cadence.par+2*delta_len+1, &p_cadence->cadence_unit.cadence_low, cadence_len);
		memcpy(rsp.cadence.par+2*delta_len+1+cadence_len, &p_cadence->cadence_unit.cadence_high, cadence_len);
		len = delta_len*2 + cadence_len*2 + 4;

		if((p_cadence->fast_period_div>MAX_FAST_CADENCE_PERIOD_DIV) || (p_cadence->cadence_unit.min_interval>MAX_MIN_INTERVAL)){
			len = OFFSETOF(sensor_cadence_st_t, cadence);
		}
	}

	return mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
}
 
int mesh_sensor_cadence_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_cadence_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0, cb_par->op_rsp, prop_id);
}

int mesh_cmd_sig_sensor_cadence_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	sensor_cadence_st_t *p_cadence = (sensor_cadence_st_t *)par;

	return mesh_sensor_cadence_st_rsp(cb_par, p_cadence->prop_id);
}

int mesh_sensor_cadence_st_publish(u8 idx)
{
	model_common_t *p_com_md = &model_sig_sensor.sensor_setup[idx].com;
	u16 ele_adr = p_com_md->ele_adr;
	u16 pub_adr = p_com_md->pub_adr;
	if(!pub_adr){
		return -1;
	}
	u8 *uuid = get_virtual_adr_uuid(pub_adr, p_com_md);
	return mesh_tx_cadence_st_rsp(idx, ele_adr, pub_adr, uuid, p_com_md, SENSOR_CANDECE_STATUS, model_sig_sensor.sensor_states[0].prop_id);
}

u8 mesh_cadence_para_check(u8 *par, int par_len)
{
	u8 err = 0;
	sensor_cadence_st_t *p_cadence = (sensor_cadence_st_t *)(par);
	sensor_data_t *p_sensor_data = get_sensor_data(p_cadence->prop_id);
	u8 var_len = p_sensor_data->len_raw;
	
	if(p_cadence->cadence.fast_period_div >= MAX_FAST_CADENCE_PERIOD_DIV){
		err = 1;
	}
	
	if(p_cadence->cadence.trig_type){
		if((par_len != var_len*2 + OFFSETOF(sensor_cadence_st_t,cadence.cadence_unitless.min_interval)+1) || (p_cadence->cadence.cadence_unitless.min_interval >= MAX_MIN_INTERVAL)){
			err = 1;
		}
	}
	else{
		if((par_len != 4+var_len*4) || (p_cadence->cadence.par[var_len*2] >= MAX_MIN_INTERVAL)){
			err = 1;
		}
	}

	return err;
}

int mesh_cmd_sig_sensor_cadence_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	int err = 0;

	sensor_cadence_st_t *p_cadence_set = (sensor_cadence_st_t *)(par);
	u16 prop_id =p_cadence_set->prop_id;
	u32 id_index = get_prop_id_index(prop_id);
	sensor_data_t *p_sensor_data = get_sensor_data(prop_id);
	u8 par_err =  mesh_cadence_para_check(par, par_len);

	if(p_sensor_data && par_err){
		return -1;
	}
	
	if(p_sensor_data){
		u8 cadence_len = p_sensor_data->len_raw;
		u8 delta_len = cadence_len;
		if(p_cadence_set->cadence.trig_type){
			delta_len = SIZEOF_MEMBER(cadence_unitless_t, delta_down);
		}
		
		if((delta_len <= SIZEOF_MEMBER(cadence_unit_t, delta_down)) && (cadence_len <= SIZEOF_MEMBER(cadence_unit_t, cadence_low))){ // cadence and delta in cadence_unit_t is u32 now
			sensor_cadence_t *p_cadence = (sensor_cadence_t *)&model_sig_sensor.sensor_states[id_index].cadence;
			memset(p_cadence, 0x00, sizeof(sensor_cadence_t));
			p_cadence->trig_type = p_cadence_set->cadence.trig_type;
			p_cadence->fast_period_div = p_cadence_set->cadence.fast_period_div;
			memcpy(&p_cadence->cadence_unit.delta_down, p_cadence_set->cadence.par, delta_len);
			memcpy(&p_cadence->cadence_unit.delta_up, p_cadence_set->cadence.par+delta_len, delta_len);
			memcpy(&p_cadence->cadence_unit.min_interval, p_cadence_set->cadence.par+2*delta_len, 1);
			memcpy(&p_cadence->cadence_unit.cadence_low, p_cadence_set->cadence.par+2*delta_len+1, cadence_len);
			memcpy(&p_cadence->cadence_unit.cadence_high, p_cadence_set->cadence.par+2*delta_len+cadence_len+1, cadence_len);
			mesh_model_store(1, SIG_MD_SENSOR_SETUP_S);

		}
	}
	
	if(cb_par->op_rsp != STATUS_NONE){
		err = mesh_sensor_cadence_st_rsp(cb_par, prop_id);
	}
	
    model_pub_check_set(ST_G_LEVEL_SET_PUB_NOW, cb_par->model, 0);
    model_pub_st_cb_re_init_sensor_setup(&mesh_sensor_cadence_st_publish); 
	return err;
}

int mesh_tx_settings_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id)
{
	sensor_settings_st_t rsp;
	u8 len = OFFSETOF(sensor_settings_st_t, setting_prop_id);
	
	rsp.prop_id = prop_id;
	u32 id_index = get_prop_id_index(prop_id);
	
	if(id_index < SENSOR_NUMS){
		memcpy(rsp.setting_prop_id, model_sig_sensor.sensor_states[id_index].setting.setting_id, sizeof(rsp.setting_prop_id));
		len = sizeof(rsp);
	}
	
	return mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
}

int mesh_sensor_settings_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_settings_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0, cb_par->op_rsp, prop_id);
}

int mesh_cmd_sig_sensor_settings_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	u16 prop_id = par[0] + (par[1]<<8);
	return mesh_sensor_settings_st_rsp(cb_par, prop_id);
}

int mesh_tx_setting_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id, u16 setting_id)
{
	if(PROHIBITED_PROP_ID == prop_id){
		return -1;
	}
	
	sensor_setting_st_t rsp;
	u8 len = OFFSETOF(sensor_setting_st_t, setting_access);
	rsp.prop_id = prop_id;
	rsp.setting_id = setting_id;
	u32 id_index = 	get_prop_id_index(prop_id);
	
	if(id_index < SENSOR_NUMS){
		sensor_setting_tbl_t *p_setting  = get_sensor_setting_tbl(id_index, setting_id);
		if(p_setting){
			rsp.setting_access = p_setting->setting_access;
			memcpy(rsp.setting_raw, p_setting->p_raw, p_setting->len_raw);
			len = len + 1 + p_setting->len_raw; // 1:access length
		}		
	}
	
	return mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
}

int mesh_sensor_setting_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id, u16 setting_id)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_setting_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0,cb_par->op_rsp, prop_id, setting_id);
}

int mesh_cmd_sig_sensor_setting_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	sensor_setting_get_t * p_setting_get = (sensor_setting_get_t *) par;
	return mesh_sensor_setting_st_rsp(cb_par, p_setting_get->prop_id, p_setting_get->setting_id);
}

int mesh_sensor_setting_st_publish_ll(u8 idx)
{
	model_common_t *p_com_md = &model_sig_sensor.sensor_setup[idx].com;
	u16 ele_adr = p_com_md->ele_adr;
	u16 pub_adr = p_com_md->pub_adr;
	if(!pub_adr){
		return -1;
	}
	u8 *uuid = get_virtual_adr_uuid(pub_adr, p_com_md);
	return mesh_tx_setting_st_rsp(idx, ele_adr, pub_adr, uuid, p_com_md, SENSOR_SETTING_STATUS, model_sig_sensor.sensor_states[0].prop_id, model_sig_sensor.sensor_states[0].setting.setting_id[0]);
}

int mesh_cmd_sig_sensor_setting_set(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	int err = 0;
	sensor_setting_set_t *p_setting_set = (sensor_setting_set_t *)par;
	u32 id_index = 	get_prop_id_index(p_setting_set->prop_id);
	
	if(id_index < SENSOR_NUMS){
		for(u8 i=0; i<SENSOR_SETTINGS_NUMS; i++){
			sensor_setting_tbl_t *p_setting_tbl  = get_sensor_setting_tbl(id_index, p_setting_set->setting_id);
			if(p_setting_tbl && (p_setting_tbl->setting_access == READ_WRITE)){
				memcpy(p_setting_tbl->p_raw, p_setting_set->setting_raw, p_setting_tbl->len_raw);
				mesh_model_store(1, SIG_MD_SENSOR_SETUP_S);
			}
		}
	}

	model_pub_check_set(ST_G_LEVEL_SET_PUB_NOW, cb_par->model, 0);
    model_pub_st_cb_re_init_sensor_setup(&mesh_sensor_setting_st_publish_ll);
	if(cb_par->op_rsp != STATUS_NONE){	
	 	err = mesh_sensor_setting_st_rsp(cb_par, p_setting_set->prop_id, p_setting_set->setting_id);
	}

	return err;
}

int mesh_tx_column_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id)
{
	if(PROHIBITED_PROP_ID == prop_id){
		return -1;	
	}
	
	sensor_series_st_t rsp;
	u8 len = OFFSETOF(sensor_series_st_t, series_raw);;
	rsp.prop_id = prop_id;
	sensor_data_t * p_sensor_data = get_sensor_data(prop_id);
	
	if(p_sensor_data){
		memcpy(&rsp.series_raw, p_sensor_data->p_series_col, sizeof(rsp.series_raw));
		len = sizeof(rsp);
	}
	
	return mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
}

int mesh_sensor_column_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_column_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0, cb_par->op_rsp, prop_id);
}

int mesh_cmd_sig_sensor_column_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	sensor_col_get_t * p_col_get = (sensor_col_get_t *) par;
	return mesh_sensor_column_st_rsp(cb_par, p_col_get->prop_id);
}

int mesh_tx_series_st_rsp(u8 idx, u16 ele_adr, u16 dst_adr, u8 *uuid, model_common_t *pub_md, u16 op_rsp, u16 prop_id)
{
	sensor_series_st_t rsp;
	u8 len = sizeof(rsp);
	
	if(PROHIBITED_PROP_ID == prop_id){
		return -1;
	}

	rsp.prop_id = prop_id;
	sensor_data_t * p_sensor_data = get_sensor_data(prop_id);

	if(p_sensor_data){
		memcpy(&rsp.series_raw, p_sensor_data->p_raw, p_sensor_data->len_raw);
	}
	else{
		memset(&rsp.series_raw, 0x00, sizeof(rsp.series_raw));
		len = OFFSETOF(sensor_series_st_t, series_raw);
	}
	
	return mesh_tx_cmd_rsp(op_rsp, (u8 *)&rsp, len, ele_adr, dst_adr, uuid, pub_md);
}

int mesh_sensor_series_st_rsp(mesh_cb_fun_par_t *cb_par, u16 prop_id)
{
	model_common_t *p_model = (model_common_t *)cb_par->model;

	return mesh_tx_series_st_rsp(cb_par->model_idx, p_model->ele_adr, cb_par->adr_src, 0, 0, cb_par->op_rsp, prop_id);
}

int mesh_cmd_sig_sensor_series_get(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	sensor_series_get_t *p_series = (sensor_series_get_t *) par;
	
	return mesh_sensor_series_st_rsp(cb_par, p_series->prop_id);
}

int mesh_sensor_st_publish(u8 idx)
{
	model_common_t *p_com_md = &model_sig_sensor.sensor_srv[idx].com;
	u16 ele_adr = p_com_md->ele_adr;
	u16 pub_adr = p_com_md->pub_adr;
	if(!pub_adr){
		return -1;
	}
	u8 *uuid = get_virtual_adr_uuid(pub_adr, p_com_md);
	return mesh_tx_sensor_st_rsp(idx, ele_adr, pub_adr, uuid, p_com_md, SENSOR_STATUS, 0, 0);
}

int mesh_sensor_setup_st_publish(u8 idx)
{
	model_common_t *p_com_md = &model_sig_sensor.sensor_setup[idx].com;
	u16 ele_adr = p_com_md->ele_adr;
	u16 pub_adr = p_com_md->pub_adr;
	if(!pub_adr){
		return -1;
	}
	u8 *uuid = get_virtual_adr_uuid(pub_adr, p_com_md);
	
	return mesh_tx_cadence_st_rsp(idx, ele_adr, pub_adr, uuid, p_com_md, SENSOR_CANDECE_STATUS, PROP_ID_PRESENT_AMBIENT_LIGHT_LEVEL);
}
#if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
void store_ambient_sensor_value(u32 data)
{
		u32 sensor_value = data;
		sensor_data_t * p_sensor_data = &sensor_data[0];
		memcpy(p_sensor_data->p_raw, &sensor_value, sizeof(sensor_value));
}
#endif


void store_occupancy_sensor_value(u8 data)
{
		u8 sensor_value = data;
        #if (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)
		sensor_data_t * p_sensor_data = &sensor_data[1];
        #else
		sensor_data_t * p_sensor_data = &sensor_data[0];
        #endif
		memcpy(p_sensor_data->p_raw, &sensor_value, sizeof(sensor_value));
}

u32 sensor_measure_proc()
{
	foreach(i, SENSOR_NUMS){ // TODO

		sensor_cadence_t *p_cadence = &model_sig_sensor.sensor_states[i].cadence;
		sensor_data_t * p_sensor_data = &sensor_data[i];

		u32 sample_interval = 1 << sensor_descrip[i].measure_period;
		if(clock_time_exceed_ms(sensor_measure_ms, sample_interval)){//
			//update sensor_measure_quantity here
		#if !WIN32
			#if SENSOR_LIGHTING_CTRL_USER_MODE_EN	// for sensor server to send sensor status.
	        gpio_set_input_en(SENSOR_GPIO_PIN, 1);
	        gpio_set_output_en(SENSOR_GPIO_PIN, 0);
	        sleep_us(100);
	        sensor_measure_quantity = gpio_read(SENSOR_GPIO_PIN) ? 0 : 1;
			#elif (((NLC_SENSOR_TYPE_SEL == NLCP_TYPE_OCS)|| (NLC_SENSOR_TYPE_SEL == NLCP_TYPE_ALS)) && (NLC_SENSOR_SEL != SENSOR_NONE))
			sensor_measure_quantity = nlc_sensor_get();
			#endif
		#endif
			if(i==0)
			{
				u8 pub_flag = 0;
				u32 measure_val = 0;
				memcpy(&measure_val, p_sensor_data->p_raw, min2(sizeof(measure_val), p_sensor_data->len_raw));

				if(sensor_measure_quantity < measure_val){
					if((measure_val - sensor_measure_quantity) > p_cadence->cadence_unit.delta_down){
						LOG_USER_MSG_INFO(0, 0, "1.sensor measure quantity:%u,measure val:%u",sensor_measure_quantity,measure_val);
						LOG_USER_MSG_INFO(0, 0, "delta down");
						pub_flag = 1;
					}
				}
				else{
					if((sensor_measure_quantity - measure_val) > p_cadence->cadence_unit.delta_up){
						LOG_USER_MSG_INFO(0, 0, "2.sensor measure quantity:%u,measure val:%u",sensor_measure_quantity,measure_val);
						LOG_USER_MSG_INFO(0, 0, "delta UP");
						pub_flag = 1;
					}
				}

				if(pub_flag){
					memcpy(p_sensor_data->p_raw, &sensor_measure_quantity, min2(sizeof(measure_val), p_sensor_data->len_raw));
				//	model_pub_check_set(ST_G_LEVEL_SET_PUB_NOW, (u8 *)&model_sig_sensor.sensor_srv[0].com, 0);
				}
			}
			else if(i==1)
			{
				u8 pub_flag = 0;
				sensor_measure_ms = clock_time_ms();
				u32 measure_val = 0;
				memcpy(&measure_val, p_sensor_data->p_raw, min2(sizeof(measure_val), p_sensor_data->len_raw));

				if(sensor_measure_quantity < measure_val){
					if((measure_val - sensor_measure_quantity) > p_cadence->cadence_unit.delta_down){
						LOG_USER_MSG_INFO(0, 0, "--1.sensor measure quantity:%u,measure val:%u",sensor_measure_quantity,measure_val);
						LOG_USER_MSG_INFO(0, 0, "--delta down");
						pub_flag = 1;
					}
				}
				else{
					if((sensor_measure_quantity - measure_val) > p_cadence->cadence_unit.delta_up){
						LOG_USER_MSG_INFO(0, 0, "--2.sensor measure quantity:%u,measure val:%u",sensor_measure_quantity,measure_val);
						LOG_USER_MSG_INFO(0, 0, "--delta UP");
						pub_flag = 1;
					}
				}

				if(pub_flag){
					memcpy(p_sensor_data->p_raw, &sensor_measure_quantity, min2(sizeof(measure_val), p_sensor_data->len_raw));
					model_pub_check_set(ST_G_LEVEL_SET_PUB_NOW, (u8 *)&model_sig_sensor.sensor_srv[0].com, 0);
				}
			}
		}
	}

	return 1;
}
#endif

#if MD_SENSOR_CLIENT_EN
int mesh_cmd_sig_sensor_descript_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	int err = 0;
	if(cb_par->model){	// model may be Null for status message
		//model_client_common_t *p_model = (model_client_common_t *)(cb_par->model);
	}
	return err;
}

#if SENSOR_LIGHTING_CTRL_USER_MODE_EN
static u32 keep_on_timer = 0;
static volatile bool sensor_set_light_on = false;

void sensor_lighting_ctrl_set_light_on()
{
    keep_on_timer = clock_time();
    sensor_set_light_on = true;
}

void sensor_lighting_ctrl_proc()
{
    static mesh_cmd_g_level_st_t level_st;
    light_g_level_get((u8 *)&level_st, 0, ST_TRANS_LIGHTNESS);

    //u8 target_onoff = get_onoff_from_level(level_st.target_level);
    u8 target_onoff = get_onoff_from_level(level_st.present_level);

    if (sensor_set_light_on) {
        if (!target_onoff) {
            u8 buf[sizeof(u16) + sizeof(mesh_cmd_g_onoff_set_t)];
            buf[0] = G_ONOFF_SET & 0xFF;
            buf[1] = (G_ONOFF_SET >> 8) & 0xFF;
            mesh_cmd_g_onoff_set_t *set = (mesh_cmd_g_onoff_set_t *)&buf[2];
            set->tid = 0;
            set->onoff = 1;
            set->transit_t = 10;
            set->delay = 10;
            mesh_tx_cmd2self_primary(0, buf, sizeof(buf));
        }else{
            if (clock_time_exceed(keep_on_timer, SENSOR_LIGHTING_CTRL_ON_MS*1000)) {
                u8 buf[sizeof(u16) + sizeof(mesh_cmd_g_onoff_set_t)];
                buf[0] = G_ONOFF_SET & 0xFF;
                buf[1] = (G_ONOFF_SET >> 8) & 0xFF;
                mesh_cmd_g_onoff_set_t *set = (mesh_cmd_g_onoff_set_t *)&buf[2];
                set->tid = 0;
                set->onoff = 0;
                set->transit_t = 10;
                set->delay = 10;
                mesh_tx_cmd2self_primary(0, buf, sizeof(buf));

                sensor_set_light_on = false;
            }
        }
    }else{
        keep_on_timer = clock_time();
    }
}
#endif

int mesh_cmd_sig_sensor_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
#if SENSOR_LIGHTING_CTRL_USER_MODE_EN	// for sensor client to receive sensor status to turn light on/off.
	sensor_mpid_B_t *p_st_B = (sensor_mpid_B_t *)par;
	if(SENSOR_DATA_FORMAT_A == p_st_B->format){
		//sensor_mpid_A_t *p_st_A = (sensor_mpid_A_t *)par;
		// TODO
	}else{ // SENSOR_DATA_FORMAT_B
		if (p_st_B->raw_value[0]) {
			sensor_lighting_ctrl_set_light_on();
		}
	}
#endif

	return 0;
}

int mesh_cmd_sig_sensor_cadence_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	return 0;
}

int mesh_cmd_sig_sensor_settings_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	return 0;
}

int mesh_cmd_sig_sensor_setting_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	return 0;
}

int mesh_cmd_sig_sensor_column_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	return 0;
}

int mesh_cmd_sig_sensor_series_status(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par)
{
	return 0;
}

int access_cmd_sensor_occupancy_motion_sensed(u16 adr_dst, u8 percent)
{
	sensor_mpid_B_t rsp;
	memset(&rsp, 0, sizeof(rsp));
	
	rsp.format = SENSOR_DATA_FORMAT_B;
	rsp.prop_id = PROP_ID_OCCUPANCY_MOTION_SENSED;
	rsp.length = 1;
	rsp.raw_value[0] = percent;
	int par_len = OFFSETOF(sensor_mpid_B_t, raw_value) + rsp.length;

	return SendOpParaDebug(adr_dst, 0, SENSOR_STATUS, (u8 *)&rsp, par_len);
}


#endif
#endif
