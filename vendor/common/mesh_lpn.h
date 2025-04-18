/********************************************************************************************************
 * @file	mesh_lpn.h
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

#include "tl_common.h"
#include "proj_lib/sig_mesh/app_mesh.h"


#define LONG_PRESS_TRIGGER_MS       (3000)
#define KEY_SCAN_INTERVAL_MS        (40)

#define LPN_GATT_OTA_ADV_STATE_MAX_TIME_MS      (30*1000)

#if LPN_DEBUG_PIN_EN
void lpn_debug_set_current_pin(u8 level);
void lpn_debug_set_debug_pin(u8 level);
void lpn_debug_set_irq_pin(u8 level);
void lpn_debug_set_event_handle_pin(u8 level);
void lpn_debug_alter_debug_pin(int reset);
#else
#define lpn_debug_set_current_pin(level)  
#define lpn_debug_alter_debug_pin(reset)  
#define lpn_debug_set_irq_pin(level) 
#define lpn_debug_set_event_handle_pin(level)   
#endif

void friend_subsc_stop();
void mesh_cmd_sig_lowpower_heartbeat();
void friend_subsc_repeat();
void lpn_set_poll_ready();
int mesh_lpn_poll_receive_timeout(void);
void friend_cmd_send_poll();
void friend_cmd_send_request();
void mesh_friend_ship_start_poll();
void mesh_feature_set_lpn();
int lpn_quick_tx(u8 is_quick_tx);
int lpn_rx_offer_handle(u8 *bear);
void lpn_no_offer_handle();
void subsc_add_rmv_test(int add);
void pts_test_case_lpn();
void lpn_subsc_list_update_by_sub_set_cmd(u16 op, u16 sub_adr);
void mesh_main_loop_LPN();
void friend_send_current_subsc_list();
void suspend_handle_next_poll_interval(int handle_type);
void suspend_handle_wakeup_rx(int handle_type);
void mesh_friend_ship_retry();
void send_gatt_adv_right_now();
u8 lpn_get_poll_retry_max();
void lpn_mode_set(int mode);
void mesh_lpn_gatt_adv_refresh();
int mesh_lpn_send_gatt_adv();
int mesh_lpn_rcv_delay_wakeup(void);
int mesh_lpn_poll_md_wakeup(void);
int mesh_lpn_adv_interval_update(u8 adv_tick_refresh);
void soft_timer_mesh_adv_proc();
void  lpn_set_sleep_wakeup (u8 e, u8 *p, int n);

//void lpn_callback_register(void *(callback_fun));
//void store_cpu_status_for_lpn(uint8_t status);
void sub_list_add(void);
/************************** access_set_lum **************************
function : cache the sublist address for lpn  
para:
	op: opcode
	addr: sublist address 
	ready: 0:send sublist control message after receive pre sublist confirm message.  1:ready to send sublist control message.
ret: 0  means OK 
	-1 or other value means err 
****************************************************************************/
int mesh_lpn_subsc_pending_add(u16 op, u16 *p_sublist, int sub_cnt, int overwrite_flag);
u32 get_lpn_poll_interval_ms();

enum{
    HANDLE_NONE                     = 0,
    HANDLE_RETENTION_DEEP_PRE       = BIT(0),
    HANDLE_RETENTION_DEEP_AFTER     = BIT(1),
    HANDLE_RETENTION_DEEP_ADV_PRE   = BIT(2),
    HANDLE_SUSPEND_NORMAL           = HANDLE_RETENTION_DEEP_PRE | HANDLE_RETENTION_DEEP_AFTER,
};

enum{
    LPN_SUSPEND_EVENT_NONE          = 0,
    LPN_SUSPEND_EVENT_NEXT_POLL_INV = 1,
    LPN_SUSPEND_EVENT_WAKEUP_RX     = 2,
};

enum{
    KEY_PRESSED_IDLE    = 0,
    KEY_PRESSED_SHORT   = 1,
    KEY_PRESSED_LONG    = 2,
};

enum{
    LPN_MODE_NORMAL         = 0,
    LPN_MODE_GATT_OTA       = 1,
};

extern u8 mesh_lpn_rx_master_key;
extern u8 fri_request_retry_max;
extern u32 lpn_wakeup_tick;
extern u8 lpn_mode;
extern u32 lpn_mode_tick;
extern u8 mesh_lpn_quick_tx_flag;

typedef struct{
	u16 op;
	u16 sub_list[SUB_LIST_MAX];
	u32  cnt;
}lpn_sub_list_event_t;

extern _align_4_ lpn_sub_list_event_t mesh_lpn_subsc_pending;
extern mesh_subsc_list_retry_t subsc_list_retry;

#define LPN_SUBSC_LIST_ADD_CNT_MAX		(SUB_LIST_MAX_IN_ONE_MSG + ARRAY_SIZE(mesh_lpn_subsc_pending.sub_list))	// default is 10.


