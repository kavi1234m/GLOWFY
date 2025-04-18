/********************************************************************************************************
 * @file	ll_adv.h
 *
 * @brief	for TLSR chips
 *
 * @author	BLE Group
 * @date	Sep. 18, 2015
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
/*
 * ll_adv.h
 *
 *  Created on: 2017-3-7
 *      Author: Administrator
 */

#ifndef LL_ADV_H_
#define LL_ADV_H_


#include "ll_whitelist.h"

/* Advertising Maximum data length */
#define 		ADV_MAX_DATA_LEN                    31

#define			BLT_ENABLE_ADV_37			BIT(0)
#define			BLT_ENABLE_ADV_38			BIT(1)
#define			BLT_ENABLE_ADV_39			BIT(2)
#define			BLT_ENABLE_ADV_ALL			(BLT_ENABLE_ADV_37 | BLT_ENABLE_ADV_38 | BLT_ENABLE_ADV_39)



typedef enum {
	BLC_ADV_DISABLE = 0x00,
	BLC_ADV_ENABLE  = 0x01,
} ll_adv_en_t;

#define			BLS_FLAG_ADV_IN_SLAVE_MODE			BIT(6)

#define 		BLC_FLAG_STK_ADV					BIT(24)







/* Advertisement Type */
typedef enum{
  ADV_TYPE_CONNECTABLE_UNDIRECTED             = 0x00,  // ADV_IND
  ADV_TYPE_CONNECTABLE_DIRECTED_HIGH_DUTY     = 0x01,  // ADV_INDIRECT_IND (high duty cycle)
  ADV_TYPE_SCANNABLE_UNDIRECTED               = 0x02 , // ADV_SCAN_IND
  ADV_TYPE_NONCONNECTABLE_UNDIRECTED          = 0x03 , // ADV_NONCONN_IND
  ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY      = 0x04,  // ADV_INDIRECT_IND (low duty cycle)
}adv_type_t;





// Advertise channel PDU Type
typedef enum advChannelPDUType_e {
	LL_TYPE_ADV_IND 		 = 0x00,
	LL_TYPE_ADV_DIRECT_IND 	 = 0x01,
	LL_TYPE_ADV_NONCONN_IND  = 0x02,
	LL_TYPE_SCAN_REQ 		 = 0x03,		LL_TYPE_AUX_SCAN_REQ 	 = 0x03,
	LL_TYPE_SCAN_RSP 		 = 0x04,
	LL_TYPE_CONNECT_REQ 	 = 0x05,		LL_TYPE_AUX_CONNECT_REQ = 0x05,
	LL_TYPE_ADV_SCAN_IND 	 = 0x06,

	LL_TYPE_ADV_EXT_IND		 = 0x07,		LL_TYPE_AUX_ADV_IND 	 = 0x07,	LL_TYPE_AUX_SCAN_RSP = 0x07,	LL_TYPE_AUX_SYNC_IND = 0x07,	LL_TYPE_AUX_CHAIN_IND = 0x07,
	LL_TYPE_AUX_CONNECT_RSP = 0x08,
} advChannelPDUType_t;




typedef struct {
	u8		adv_chn_mask;
	u8		adv_duraton_en;
	u8		adv_type;
	u8 		adv_filterPolicy;

	u8		T_SCAN_RSP_INTVL;
	u8      own_addr_type;     //own_addr_type
	u8		rsvd;
	u8		advInt_rand;

	u32		advInt_min;
	u32 	advInt_max;

	u32		adv_interval;     // system tick
	u32		adv_duration_us;
	u32		adv_begin_tick;

#if (LL_FEATURE_ENABLE_LL_PRIVACY)
	u8      advPeerAddrType;
	u8		advRpaTmoFlg;
	s8      advRpaRlIdx;
	u8      advRpaResoved;

	u16     rsvd1;
	u8		advPeerAddr[BLE_ADDR_LEN];
#endif
}st_ll_adv_t;



extern _attribute_aligned_(4) st_ll_adv_t  blta;


extern rf_packet_adv_t	pkt_adv;


typedef int (*ll_adv2conn_callback_t)(u8 *, bool);   //rcvd conn_req, adv state to conn state
extern 	ll_adv2conn_callback_t ll_adv2conn_cb;


typedef int  (*ll_module_adv_callback_t)(void);



typedef int (*advertise_prepare_handler_t) (rf_packet_adv_t * p);


extern u32  blc_rcvd_connReq_tick;



/******************************* User Interface  ************************************/
void 		blc_ll_initAdvertising_module(u8 *public_adr);;


ble_sts_t	bls_ll_setAdvData(u8 *data, u8 len);
ble_sts_t 	bls_ll_setScanRspData(u8 *data, u8 len);
ble_sts_t   bls_ll_setAdvEnable(int adv_enable);

u8 			blt_set_adv_direct_init_addrtype(u8* cmdPara);

ble_sts_t   bls_ll_setAdvParam( u32 intervalMin,  u32 intervalMax,  adv_type_t advType,  		 own_addr_type_t ownAddrType,  \
							     u8 peerAddrType, u8  *peerAddr,    u8 		   adv_channelMap,   adv_fp_type_t   advFilterPolicy);


ble_sts_t 	bls_ll_setAdvInterval(u32 intervalMin, u32 intervalMax);
ble_sts_t 	bls_ll_setAdvChannelMap(u8 adv_channelMap);
ble_sts_t 	bls_ll_setAdvFilterPolicy(adv_fp_type_t advFilterPolicy);

ble_sts_t   bls_ll_setAdvDuration (u32 duration_us, u8 duration_en);


void 		blc_ll_setAdvCustomedChannel (u8 chn0, u8 chn1, u8 chn2);




ble_sts_t   blc_ll_addAdvertisingInConnSlaveRole(void);
ble_sts_t   blc_ll_removeAdvertisingFromConnSLaveRole(void);
ble_sts_t 	blc_ll_setAdvParamInConnSlaveRole( u8 		  *adv_data,  u8              advData_len, u8 *scanRsp_data,  u8 scanRspData_len,
											   adv_type_t  advType,   own_addr_type_t ownAddrType, u8 adv_channelMap, adv_fp_type_t advFilterPolicy);


static inline u32 	bls_ll_getConnectionCreateTime(void)
{
	return blc_rcvd_connReq_tick;
}
void blt_adv_expect_time_refresh(u8 en);

/************************* Stack Interface, user can not use!!! ***************************/
ble_sts_t 	bls_hci_le_setAdvParam(adv_para_t *para);
ble_sts_t 	bls_hci_le_readChannelMap(u16 connHandle, u8 *returnChannelMap);


ble_sts_t   bls_ll_setAdvType(u8 advType);
ble_sts_t 	blt_set_adv_addrtype(u8* cmdPara);

static inline u8 	blt_ll_getOwnAddrType(void)
{
	return blta.own_addr_type;
}

extern u8 blc_continue_adv_en;//default stop sending adv packets when receiving scan request in the current adv interval.
/**
 * @brief      this function is used to set whether to continue sending broadcast packets when receiving scan request in the current adv interval.
 * @param	   enable - enable:continue sending broadcast packets when receiving scan request.
 * @return     none.
 */
static inline void bls_ll_continue_adv_after_scan_req(u8 enable){
	blc_continue_adv_en = enable;
}
#endif /* LL_ADV_H_ */
