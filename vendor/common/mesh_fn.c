/********************************************************************************************************
 * @file	mesh_fn.c
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
#include "proj_lib/ble/blt_config.h"
#include "vendor/common/user_config.h"
#include "app_health.h"
#include "proj_lib/sig_mesh/app_mesh.h"
#include "lighting_model.h"
#include "vendor/common/sensors_model.h"
#include "proj_lib/mesh_crypto/sha256_telink.h"
#include "mesh_common.h"
#include "vendor/common/app_health.h"
#include "vendor/common/app_heartbeat.h"
#include "directed_forwarding.h"

// ------- friend node && LPN common
u8 mesh_subsc_adr_cnt_get (mesh_cmd_bear_t *p_br)
{
    u32 len_lt = mesh_lt_len_get_by_bear(p_br);
    return (len_lt - 1)/2;
}

int fn_cache_get_extend_adv_short_unseg(u32 ctl, u8 len_ut_max_single_seg_rx)
{
    #if (MESH_DLE_MODE == MESH_DLE_MODE_EXTEND_BEAR)
    if(len_ut_max_single_seg_rx == (mesh_max_payload_get(ctl, 1))){
        return 1; // keep the same max bear length
    }
    #endif
    
    return 0;
}

void friend_cmd_send_fn(u8 lpn_idx, u8 op)  // always need.
{
#if (FRI_SAMPLE_EN)
	friend_cmd_send_sample_message(op);
#else
    if(CMD_CTL_HEARTBEAT == op){
		heartbeat_cmd_send_conf();
	}else if(CMD_CTL_CLEAR == op){
	#if (FEATURE_FRIEND_EN || FEATURE_LOWPOWER_EN)
		mesh_ctl_fri_clear_t fri_clear;
		u16 adr_dst = 0;
		#if FEATURE_LOWPOWER_EN
		fri_clear.LPNAdr = mesh_lpn_par.LPNAdr;
		fri_clear.LPNCounter = 2;       // confirm later, should use parameters in last request command.
		adr_dst = mesh_lpn_par.FriAdr;  // confirm later, should use parameters in last request command.
		#elif FEATURE_FRIEND_EN
		fri_clear.LPNAdr = fn_other_par[lpn_idx].LPNAdr;
		fri_clear.LPNCounter = fn_req[lpn_idx].LPNCounter;
		adr_dst = fn_req[lpn_idx].PreAdr;
		#endif
        friend_cmd_send_clear(adr_dst, (u8 *)&fri_clear, sizeof(mesh_ctl_fri_clear_t));
    #endif
    }
	#if FEATURE_FRIEND_EN
	else if(CMD_CTL_OFFER == op){
        #if FRIEND_MSG_TEST_EN
        if(mesh_adv_tx_cmd_sno < 0x014820){
            mesh_adv_tx_cmd_sno = 0x014820; // for test
        }
        #endif
        friend_cmd_send_offer(lpn_idx);
    }else if(CMD_CTL_UPDATE == op){
        #if FRIEND_MSG_TEST_EN
        if(mesh_adv_tx_cmd_sno < 0x014834){
            mesh_adv_tx_cmd_sno = 0x014834; // for test
        }
        #endif
        friend_cmd_send_update(lpn_idx, 0);
    }
    #endif
#endif
}


#if (FEATURE_FRIEND_EN || FEATURE_LOWPOWER_EN)
// cache data should be not encryption. p_bear should have been big endianness
// @ par: len:  from "trans_par_val"
void mesh_friend_logmsg(mesh_cmd_bear_t *p_bear_big, u8 len)
{
	#if (DEBUG_PROXY_FRIEND_SHIP && WIN32)
	extern u8 log_en_lpn1,log_en_lpn2;
	u16 adr_dst = p_bear_big->nw.dst;
	endianness_swap_u16((u8 *)&adr_dst);
	if((log_en_lpn1 && (adr_dst == (PROXY_FRIEND_SHIP_MAC_LPN1 & 0xff)))
	 || (log_en_lpn2 && (adr_dst == (PROXY_FRIEND_SHIP_MAC_LPN2 & 0xff)))){
		LOG_MSG_WARN(TL_LOG_FRIEND,(u8 *)&p_bear_big->trans_par_val, min(26,len),"mesh_friend_logmsg:send message to LPN_win32: \r\n");
	}
	#else
	LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)&p_bear_big->trans_par_val, min(26,len),"mesh_friend_logmsg:send message to LPN: \r\n");
	#endif
}

void friend_subsc_list_add_adr(lpn_adr_list_t *adr_list_src, lpn_adr_list_t *adr_list_add, u32 cnt)
{
    // should not use u16* as parameter,because it is not sure 2bytes aligned
    foreach(i,cnt){
        int exit_flag = 0;
        foreach(j,SUB_LIST_MAX_LPN){
            if(adr_list_add->adr[i] == adr_list_src->adr[j]){
                exit_flag = 1;
                break;
            }
        }
        
        if(!exit_flag){
            foreach(k,SUB_LIST_MAX_LPN){
                if(0 == adr_list_src->adr[k]){
                    adr_list_src->adr[k] = adr_list_add->adr[i];
					#if (MD_DF_CFG_SERVER_EN && !FEATURE_LOWPOWER_EN && !WIN32)
					directed_forwarding_solication_start(mesh_key.netkey_sel_dec, (mesh_ctl_path_request_solication_t *)&adr_list_src->adr[k], 1);
					#endif
					break;
                }
            }
        }
    }
}

// use 'inline' should be better. 
void friend_subsc_list_rmv_adr(lpn_adr_list_t *adr_list_src, lpn_adr_list_t *adr_list_rmv, u32 cnt)
{
    // should not use u16* as parameter,because it is not sure 2bytes aligned
    foreach(i,cnt){
        foreach(j,SUB_LIST_MAX_LPN){
            if(adr_list_rmv->adr[i] == adr_list_src->adr[j]){
                adr_list_src->adr[j] = 0;
                //break;
            }
        }
    }
}

void friend_cmd_send_clear(u16 adr_dst, u8 *par, u32 len)
{	
	LOG_MSG_LIB(TL_LOG_FRIEND,par, len,"send friend clear:");
    mesh_tx_cmd_layer_upper_ctl(CMD_CTL_CLEAR, par, len, ele_adr_primary, adr_dst,0);
}


#if TEST_CASE_HBS_BV_05_EN
void friend_add_special_grp_addr()
{
	u16 sub_adr = 0xc100;
	friend_subsc_list_add_adr(fn_other_par[0].SubsList, &sub_adr, 1);
	return ;
}
#endif

void fn_quick_send_adv()
{
#ifndef WIN32
	u32 r = irq_disable(); // should be disable, otherwise cause too much delay.(bltParam.adv_scanReq_connReq will be true and return from irq_blt_sdk_handler directedly if interrupt trigger)
    mesh_send_adv2scan_mode(1);
    irq_restore(r);
#endif
}
#endif

// ------- friend node
#if (FEATURE_FRIEND_EN)
u8 get_tx_nk_arr_idx_friend(u16 adr, u16 op)
{
	u8 nk_arr_idx = 0;
    if(is_fn_support_and_en && adr && is_unicast_adr(adr)){
        foreach(i, g_max_lpn_num){
            if(is_ele_in_node(adr, fn_other_par[i].LPNAdr, fn_req[i].NumEle)){
                if((CMD_CTL_OFFER == op) || is_in_mesh_friend_st_fn(i)){
                    if(fn_other_par[i].nk_sel_dec_fn < NET_KEY_MAX){
                        nk_arr_idx = fn_other_par[i].nk_sel_dec_fn;
                    }
                }
                break;
            }
        }
    }
    return nk_arr_idx;
}

u16 mesh_mac_match_friend(u16 adr)
{
	u16 match_F2L = 0;
    if(is_fn_support_and_en && is_unicast_adr(adr)){
        foreach(i, g_max_lpn_num){
            if(is_in_mesh_friend_st_fn(i) && is_ele_in_node(adr, fn_other_par[i].LPNAdr, fn_req[i].NumEle)){
                match_F2L |= BIT(i);
                break;
            }
        }
    }
    return match_F2L;
}

u16 mesh_group_match_friend(u16 adr)
{
	u16 match_F2L = 0;
    if(is_fn_support_and_en){
        foreach(i, g_max_lpn_num){
            if(is_in_mesh_friend_st_fn(i)){
                foreach_arr(m,fn_other_par[i].SubsList){
                    if(fn_other_par[i].SubsList[m] == adr){
                        match_F2L |= BIT(i);
                        break;
                    }
                }
            }
        }
    }
    return match_F2L;
}

int is_unicast_friend_msg_from_lpn(mesh_cmd_nw_t *p_nw)
{
	foreach(i,g_max_lpn_num){
	    if(is_in_mesh_friend_st_fn(i)
	        && (fn_other_par[i].LPNAdr == p_nw->src)
	        && (fn_other_par[i].FriAdr == p_nw->dst)){    // must because of group address
	        return 1;
	    }
	}
	return 0;
}

int is_unicast_friend_msg_to_lpn(mesh_cmd_nw_t *p_nw)
{
	if(is_unicast_adr(p_nw->dst)){
		foreach(i,g_max_lpn_num){
		    if(is_in_mesh_friend_st_fn(i)
		        && (fn_other_par[i].LPNAdr == p_nw->dst)
		        && (fn_other_par[i].FriAdr == p_nw->src)){    // must because of relay message
		        return 1;
		    }
		}
	}
	return 0;
}

int is_cmd2lpn(u16 adr_dst)
{
	foreach(i,g_max_lpn_num){
		if(fn_other_par[i].LPNAdr == adr_dst){
			return 1;
		}
	}
	return 0;
}

int friend_cache_check_replace(u8 lpn_idx, mesh_cmd_bear_t *bear_big)
{
    int replace = FN_CACHE_REPLACE_NONE; //0;
	u32 r = irq_disable();
    mesh_cmd_bear_t *p_buf_bear;
    my_fifo_t *p_cache_fifo = fn_other_par[lpn_idx].p_cache;
    u8 cnt = my_fifo_data_cnt_get(p_cache_fifo);
	
    foreach(i,cnt){
		p_buf_bear = (mesh_cmd_bear_t *)my_fifo_get_offset(p_cache_fifo, i);
		// is big endianness in cache buff
		if(p_buf_bear->nw.ctl){
            u8 op_buf = p_buf_bear->lt_ctl_unseg.opcode;
    		if(CMD_CTL_UPDATE == op_buf){
    			if(0 == i){
					// if "update" is the last one, replaced should be better, because "update" was already pushed to fifo when receive poll, 
					// and in the next 150ms may tx command such as onoff with destination address 0xffff, and this command will be both tx with master key, push to cache.
					// if LPN receive this master key onoff message, FSN will toggle, FN will pop "Update", but then the onoff message with friend key
					// will be ignored because sno is the old, so it will cause LPN retry poll 8 times till toggle FSN.
					if(cnt == 1){
						replace = FN_CACHE_REPLACE_AND_OVERWRITE;
						my_fifo_pop(p_cache_fifo);
						my_fifo_push_adv(p_cache_fifo, (u8 *)bear_big, mesh_bear_len_get(bear_big), 0);
					}else{
						// if bear_big is  CMD_CTL_ACK, and there is CMD_CTL_ACK in fofo, "update" should not replaced here.
					}
    			}else{
    				// only happen in V1.0 draft spec.
                	// should not replace directly, because of sno cache, and can be discarded only.
					// it will be discarded in "FN Cache retry" of mesh_friend_ship_proc_FN_() when receive "POLL".
				}
    		}else if(CMD_CTL_ACK == op_buf){// the first one should be also replaced.
			    if(bear_big->nw.ctl && (CMD_CTL_ACK == bear_big->lt_ctl_unseg.opcode)){
    				if((bear_big->nw.src == p_buf_bear->nw.src) && (p_buf_bear->lt_ctl_ack.seqzero == bear_big->lt_ctl_ack.seqzero)){// replace old seg ack						
						replace = FN_CACHE_REPLACE_FINISH;
						if(!memcmp(&p_buf_bear->lt_ctl_ack.seg_map, &bear_big->lt_ctl_ack.seg_map, 4)){
							break;
						}
						
						if(0 == i){// take it have been send
							replace = FN_CACHE_REPLACE_AND_OVERWRITE;
							my_fifo_pop(p_cache_fifo);
							my_fifo_push_adv(p_cache_fifo, (u8 *)bear_big, mesh_bear_len_get(bear_big), 0);
						}
						else{
	    				 	// memcpy(p_buf_bear->nw.sno, bear_big->nw.sno, 3); // should not change sno because sno cache.
	                        memcpy(&p_buf_bear->lt_ctl_ack.seg_map, &bear_big->lt_ctl_ack.seg_map, 4);
						}
						break;
    				}
    			}
    		}
		}
	}
	irq_restore(r);
    return replace;
}

mesh_fri_ship_other_t * mesh_fri_cmd2cache(u8 *p_bear_big, u8 len_nw, u8 adv_type, u8 trans_par_val, u16 F2L_bit)
{
    int err = 0;
    mesh_cmd_bear_t *p_br_big = (mesh_cmd_bear_t *)p_bear_big;
    mesh_fri_ship_other_t *p_other = 0;
    
    // fifo packet
    p_br_big->trans_par_val = trans_par_val;
    p_br_big->len = len_nw + 1;
    p_br_big->type = adv_type;

    foreach(i,g_max_lpn_num){
    	if(F2L_bit & BIT(i)){
    	    p_other = &fn_other_par[i];
		    if(!p_other->p_cache){
		        while_1_test;
		    }
		    
		    // big endianness and not encryption in FIFO
		    int replace = friend_cache_check_replace(i, p_br_big);
		    if(FN_CACHE_REPLACE_NONE == replace){
				int overwrite = (my_fifo_free_cnt_get(p_other->p_cache) == 0);
		        err = my_fifo_push_adv(p_other->p_cache, (u8 *)p_br_big, mesh_bear_len_get(p_br_big), 1);
				if(!err){
					if(overwrite){
						p_other->cache_overwrite = 1;
					}
		        }else{
					p_other = 0;
		        }
		    }else if(FN_CACHE_REPLACE_AND_OVERWRITE == replace){
                p_other->cache_overwrite = 1; // because fn record the rsp pointer when receive poll, and sent this rsp pointer directly when 150ms arrive. so need to re-get cache when there is pop action in friend_cache_check_replace_() 
		    }
	    }
    }
    return p_other;
}

void mesh_iv_update_start_poll_fn(u8 iv_update_by_sno, u8 beacon_iv_update_pkt_flag)
{
    foreach(i, g_max_lpn_num){
        if(is_in_mesh_friend_st_fn(i)){
            if(iv_update_by_sno || beacon_iv_update_pkt_flag){
                friend_cmd_send_update(i, 1);//friend_cmd_send_fn(CMD_CTL_UPDATE);
            }
        }
    }
}

int is_friend_ship_link_ok_fn(u8 lpn_idx)
{
    return fn_other_par[lpn_idx].link_ok;
}

void friend_cmd_send_offer(u8 lpn_idx)
{
	LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)(fn_offer+lpn_idx), sizeof(mesh_ctl_fri_offer_t),"send friend offer:");
    fn_offer[lpn_idx].FriCounter++; // must before
    mesh_tx_cmd_layer_upper_ctl_FN(CMD_CTL_OFFER, (u8 *)(fn_offer+lpn_idx), sizeof(mesh_ctl_fri_offer_t), fn_other_par[lpn_idx].LPNAdr);
    mesh_friend_key_update_all_nk(lpn_idx, fn_other_par[lpn_idx].nk_sel_dec_fn);
}

void friend_cmd_send_update(u8 lpn_idx, u8 md)
{
    mesh_net_key_t *p_netkey = &mesh_key.net_key[fn_other_par[lpn_idx].nk_sel_dec_fn][0];
    get_iv_update_key_refresh_flag(&fn_update[lpn_idx].flag, fn_update[lpn_idx].IVIndex, p_netkey->key_phase);
    fn_update[lpn_idx].md = !!md;
    LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)(fn_update+lpn_idx), sizeof(mesh_ctl_fri_update_t),"send friend update:");
    mesh_tx_cmd_layer_upper_ctl_FN(CMD_CTL_UPDATE, (u8 *)(fn_update+lpn_idx), sizeof(mesh_ctl_fri_update_t), fn_other_par[lpn_idx].LPNAdr);
}

void friend_cmd_send_clear_conf(u16 adr_dst, u8 *par, u32 len)
{
	LOG_MSG_LIB(TL_LOG_FRIEND,par, len,"send friend clear confirm:");
    mesh_tx_cmd_layer_upper_ctl_FN(CMD_CTL_CLR_CONF, par, len, adr_dst);	
}

void friend_cmd_send_subsc_conf(u16 adr_dst, u8 transNo)
{
	LOG_MSG_LIB(TL_LOG_FRIEND, (u8 *)&transNo, 1,"send friend sub list confirm:");
    use_mesh_adv_fifo_fn2lpn = 1;
    mesh_tx_cmd_layer_upper_ctl_FN(CMD_CTL_SUBS_LIST_CONF, (u8 *)&transNo, 1, adr_dst);
    use_mesh_adv_fifo_fn2lpn = 0;
}

u8 fn2lpn_no_retransmit = 0;
int mesh_tx_cmd_add_packet_fn2lpn(u8 *p_bear)
{
    #if WIN32	// WIN32 can't use mesh_adv_fifo_fn2lpn, because not call app_advertise_prepare_handler_
    use_mesh_adv_fifo_fn2lpn = 0;
    return mesh_tx_cmd_add_packet(p_bear);
    #else
    mesh_cmd_bear_t *p = (mesh_cmd_bear_t *)p_bear;
    if(PTS_TEST_EN || fn2lpn_no_retransmit){
	    p->trans_par_val = 0x10;    // FN-BV16 / BV19 require no more than NW PDU. confirm later.
	}
	LOG_MSG_INFO(TL_LOG_MESH,0, 0,"mesh_tx_cmd_add_packet_fn2lpn");
    int err = my_fifo_push_adv(&mesh_adv_fifo_fn2lpn, p_bear, mesh_bear_len_get(p), 0);

    fn_quick_send_adv();

    return err;
    #endif
}

void mesh_rc_segment_handle_fn(mesh_match_type_t *p_match_type, mesh_cmd_nw_t *p_nw)
{
    if(mesh_cmd_action_need_friend(p_match_type)){
        #if FN_PRIVATE_SEG_CACHE_EN
        while(is_friend_seg_cache_busy()){  // push all cache of last segment to friend cache.
            friend_seg_cache2friend_cache(p_match_type->F2L);
        }
        
        mesh_rc_segment2fri_segment(p_nw);
        #else
        mesh_rc_segment2friend_cache(p_nw, p_match_type->F2L);
        #endif
        SEG_DEBUG_LED(1);
    }
}

int mesh_get_val_with_factor(int val, u32 fac)
{
    int result;
    if(1 == fac){
        result = (val * 3) / 2;
    }else if(2 == fac){
        result = (val * 2);
    }else if(3 == fac){
        result = (val * 5) / 2;
    }else{
        result = val;
    }
    return result;
}

#define get_min_cache_size_by_log(n)    (1 << n)

int mesh_friend_request_is_valid(mesh_ctl_fri_req_t *p_req)
{
    return (/*(!p_req->Criteria.RFU)&&*/(p_req->Criteria.MinCacheSizeLog)&&(p_req->RecDelay >= 0x0A)
          &&((p_req->PollTimeout >= 0x0A)&&(p_req->PollTimeout <= 0x34BBFF))&&p_req->NumEle);
}

#if 0
u32 get_poll_timeout_fn(u16 lpn_adr)
{
    foreach_arr(i,fn_other_par){
        if(is_friend_ship_link_ok_fn(i) && (lpn_adr == fn_other_par[i].LPNAdr)){
            return fn_req[i].PollTimeout;
        }
    }
    return 0;
}
#endif

u32 get_current_poll_timeout_timer_fn(u16 lpn_adr)
{
    foreach(i,MAX_LPN_NUM){
        if(is_friend_ship_link_ok_fn(i) && (lpn_adr == fn_other_par[i].LPNAdr)){
            u32 timeout_100ms = fn_req[i].PollTimeout;
            if (timeout_100ms != 0x000000) {
            	u32 span_100ms = (clock_time_100ms()|1) - fri_ship_proc_fn[i].poll_tick;
            	if(timeout_100ms > span_100ms){
            	    timeout_100ms -= span_100ms;
            	}else{
            	    timeout_100ms = 0;
            	}
            }
            return timeout_100ms;
        }
    }
    return 0;
}

static inline int mesh_friend_poll_is_valid(mesh_ctl_fri_poll_t *p_poll)
{
    return (0 == p_poll->RFU);
}

inline void mesh_friend_ship_set_st_fn(u8 lpn_idx, u8 st)
{
    fri_ship_proc_fn[lpn_idx].status = st;
}

void mesh_friend_ship_proc_init_fn(u8 lpn_idx)
{
    memset(&fri_ship_proc_fn[lpn_idx], 0, sizeof(mesh_fri_ship_proc_fn_t));
}

void mesh_friend_ship_clear_FN(u8 lpn_idx)
{
	mesh_friend_ship_proc_init_fn(lpn_idx);
    memset(fn_other_par+lpn_idx, 0, sizeof(fn_other_par[0]));
    memset(fn_req+lpn_idx, 0, sizeof(fn_req[0]));
    memset(fn_poll+lpn_idx, 0, sizeof(fn_poll[0]));
    memset(fn_ctl_rsp_delay+lpn_idx, 0, sizeof(fn_ctl_rsp_delay[0]));
    mesh_fri_cache_fifo[lpn_idx].wptr = mesh_fri_cache_fifo[lpn_idx].rptr = 0;
}

void friend_ship_disconnect_fn(u8 lpn_idx, int type)
{
    LOG_MSG_LIB(TL_LOG_FRIEND,0, 0,"Friend ship disconnect, LPN addr:0x%04x, type: %d ", fn_other_par[lpn_idx].LPNAdr, type);
    friend_ship_disconnect_cb_fn(lpn_idx, type);
    mesh_friend_ship_clear_FN(lpn_idx);
}

void mesh_friend_ship_init_all()
{
    foreach(i,g_max_lpn_num){
        mesh_friend_ship_clear_FN(i);
    }
}

inline u8* mesh_friend_ship_cache_check(my_fifo_t *f)
{
    return my_fifo_get(f);
}

int is_poll_cmd(mesh_cmd_nw_t *p_nw)
{
    mesh_cmd_bear_t *p_bear = CONTAINER_OF(p_nw,mesh_cmd_bear_t,nw);
    return (p_bear->nw.ctl && (CMD_CTL_POLL == p_bear->lt_ctl_unseg.opcode));
}

int is_in_mesh_friend_st_fn(u8 lpn_idx)
{
	if(is_fn_support_and_en){
    	return (is_friend_ship_link_ok_fn(lpn_idx) && (fn_other_par[lpn_idx].LPNAdr != 0));
	}else{
		return 0;
	}
}

#if GATEWAY_ENABLE
int is_in_mesh_friend_st_fn_all()
{
	for(int i=0;i<MAX_LPN_NUM;i++){
		if(is_in_mesh_friend_st_fn(i)){
			return 1;
		}
	}
	return 0;
}
#endif

u32 mesh_friend_local_delay(u8 lpn_idx)		// for FN
{
    int t_delay_org = mesh_get_val_with_factor(fn_offer[lpn_idx].RecWin, fn_req[lpn_idx].Criteria.RecWinFac) // default is: 20 * 1.5 = 30ms
                - mesh_get_val_with_factor(fn_offer[lpn_idx].RSSI, fn_req[lpn_idx].Criteria.RSSIFac);

	int t_delay = t_delay_org;
	int max_delay = FRI_ESTABLISH_PERIOD_MS - 20; // 20: max random + margin.
	int min_delay = FRI_ESTABLISH_REC_DELAY_MS;
	if(t_delay > max_delay){
		if(max_delay > min_delay){
			#if (FRI_ESTABLISH_WIN_MS == 1000) // fpllow spec
			t_delay = max_delay;
			#else
			t_delay = min_delay + rand() % (max_delay - min_delay);
			#endif
		}else{
			t_delay = min_delay;
		}
    }
    
    if(t_delay < min_delay){
        t_delay = min_delay;    // spec required in section of "3.6.6.3.1 Friend establishment".
    }
    
	LOG_MSG_LIB(TL_LOG_FRIEND, 0, 0,"offer rssi: %d, delay: %d, org: %d", fn_offer[lpn_idx].RSSI, t_delay, t_delay_org);

    return t_delay;
}

inline void mesh_stop_clear_cmd(u8 lpn_idx)
{
	#if 1
	fri_ship_proc_fn[lpn_idx].clear_poll = 0;
	#else
	mesh_friend_ship_proc_init_fn(lpn_idx);
	#endif
}

void mesh_reset_poll_timeout_timer(u8 lpn_idx)
{
    fri_ship_proc_fn[lpn_idx].poll_tick = clock_time_100ms();
}

enum{
    DELAY_POLL = 1,
    DELAY_SUBSC_LIST,
    DELAY_CLEAR_CONF,
};

void mesh_friend_set_delay_par_poll(u8 lpn_idx, u8 *rsp, u32 timeStamp)
{
    fn_ctl_rsp_delay_t *p_delay = &fn_ctl_rsp_delay[lpn_idx];
    p_delay->delay_type = DELAY_POLL;
    p_delay->tick = timeStamp;
    p_delay->poll_rsp = rsp;

	mesh_friend_logmsg((mesh_cmd_bear_t *)rsp, ((mesh_cmd_bear_t *)rsp)->len + 2);
}

void mesh_friend_set_delay_par(u8 delay_type, u8 lpn_idx, u16 adr_dst, u16 par_val)
{
    fn_ctl_rsp_delay_t *p_delay = &fn_ctl_rsp_delay[lpn_idx];
    p_delay->delay_type = delay_type;
    p_delay->tick = clock_time();
    p_delay->adr_dst = adr_dst;
    p_delay->par_val = par_val;
}

u8 * get_cache_buf_for_poll(u32 lpn_idx, int poll_retry_flag, int check_again_flag)
{
	u32 i = lpn_idx;
	my_fifo_t *f_cache = fn_other_par[i].p_cache;
	u8 *p_br_cache = mesh_friend_ship_cache_check(f_cache);
	
	if(0 == poll_retry_flag){
		if(p_br_cache){
			if(fn_other_par[i].cache_overwrite){
				fn_other_par[i].cache_overwrite = 0;
			}else{
				my_fifo_pop(f_cache);
			}
		}
		p_br_cache = mesh_friend_ship_cache_check(f_cache);
		if(p_br_cache){
			LOG_MSG_LIB(TL_LOG_FRIEND,p_br_cache+OFFSETOF(mesh_cmd_bear_t, len), 16,"FN Cache message of NW(Big endian):");
		}else{
			#if FN_PRIVATE_SEG_CACHE_EN
			if(is_friend_seg_cache_busy()){
				friend_seg_cache2friend_cache(BIT(i));
			}else
			#endif
			{
				friend_cmd_send_fn(i, CMD_CTL_UPDATE);	  // push to cache
			}
			p_br_cache = mesh_friend_ship_cache_check(f_cache);
		}
	}else{
		if(check_again_flag){
			LOG_MSG_LIB(TL_LOG_FRIEND,0, 0,"FN Cache check again");
		}else{
			LOG_MSG_LIB(TL_LOG_FRIEND,0, 0,"FN Cache retry");
		}
		
		if(fn_other_par[i].cache_overwrite){
			fn_other_par[i].cache_overwrite = 0;
		}
		
		if(my_fifo_data_cnt_get(f_cache) > 1){
			mesh_cmd_bear_t *p_bear_tmp = (mesh_cmd_bear_t *)p_br_cache;
			if(p_bear_tmp->nw.ctl && (CMD_CTL_UPDATE == p_bear_tmp->lt_ctl_unseg.opcode)){
				my_fifo_pop(f_cache);
				p_br_cache = mesh_friend_ship_cache_check(f_cache);
			}
		}					
	}

	return p_br_cache;
}

static inline int is_change_iv_just_before_push_cache(fn_ctl_rsp_delay_t *p_delay)
{
	int iv_change_flag = 0;
#if 0
	// can not use this method, because "poll" may be retry, and then iv_tx will not be the same as iv before, 
	// then mesh_friend_set_delay_par_poll_ will not record correct iv_low, then fail to return iv_change_flag as 1.
	if(p_delay->iv_low != (iv_idx_st.iv_tx & 0xff))
#else
	mesh_cmd_bear_t *p_org_big = (mesh_cmd_bear_t *)p_delay->poll_rsp; // be careful, poll_rsp is big endianness here.
	u32 sno_cache_little = 0;
	u16 src_addr_little = 0;
	swap24((u8 *)&sno_cache_little, p_org_big->nw.sno); 			// get little endianness of sno.
	swap16((u8 *)&src_addr_little, (u8 *)&p_org_big->nw.src);				// get little endianness of src.
	//LOG_MSG_LIB(TL_LOG_FRIEND,0, 00,"yy, cache:0x%x, tx:0x%x, src:0x%x", sno_cache_little, mesh_adv_tx_cmd_sno, src_addr_little);
	if(sno_cache_little > mesh_adv_tx_cmd_sno && is_own_ele(src_addr_little))
#endif
	{
		// may happen when iv is changed just after push data to poll_rsp.
		iv_change_flag = 1;
		//LOG_MSG_LIB(TL_LOG_FRIEND,0, 00,"change iv just before push data");
	}

	return iv_change_flag;
}

void mesh_friend_response_delay_proc_fn(u8 lpn_idx)
{
    fn_ctl_rsp_delay_t *p_delay = &fn_ctl_rsp_delay[lpn_idx];
    int print_cache_flag = 0;
    
    if(p_delay->delay_type && clock_time_exceed(p_delay->tick, fn_req[lpn_idx].RecDelay * 1000 - 1800)){    // 1800us: encryption pkt time
        if(DELAY_POLL == p_delay->delay_type){
            if(p_delay->poll_rsp){
            	if(fn_other_par[lpn_idx].cache_overwrite){
            		p_delay->poll_rsp = get_cache_buf_for_poll(lpn_idx, 1, 1); // cache_overwrite will be clear inside.
            	}
				
				u8 bear_temp_buf[MESH_BEAR_SIZE];
                mesh_cmd_bear_t *bear_temp = (mesh_cmd_bear_t *)bear_temp_buf; // TODO DLE
                memcpy(bear_temp, p_delay->poll_rsp, min(MESH_BEAR_SIZE, mesh_bear_len_get((mesh_cmd_bear_t *)p_delay->poll_rsp)));
				u8 bear_tx_len = mesh_bear_len_get(bear_temp);
                if(bear_tx_len <= MESH_BEAR_SIZE){
					print_cache_flag = 1; // should not print here, because tx message to LPN should not be delay.
					int iv_change_flag = is_change_iv_just_before_push_cache(p_delay);
					if(iv_change_flag){
						iv_idx_st.iv_tx--;
					}
					// no encryption before, because need to check buffer in mesh_fri_cmd2cache_(), then to set cache_overwrite or not.
                    mesh_sec_msg_enc_nw_rf_buf((u8 *)(&bear_temp->nw), mesh_lt_len_get_by_bear(bear_temp), FRIENDSHIP, lpn_idx,0,fn_other_par[lpn_idx].nk_sel_dec_fn, 0);
					if(iv_change_flag){
						iv_idx_st.iv_tx++; // restore
					}
					
                    mesh_tx_cmd_add_packet_fn2lpn((u8 *)bear_temp);
                }else{
                    LOG_MSG_ERR(TL_LOG_MESH,0, 0 ,"fn rsp len err");
                    //  TODO: DLE
                }
            }

            mesh_fri_ship_proc_fn_t *proc_fn = &fri_ship_proc_fn[lpn_idx];
            if(proc_fn->clear_delay_cnt){
                proc_fn->clear_delay_cnt--;
                if(0 == proc_fn->clear_delay_cnt){ // make sure establish friend ship success
                    friend_cmd_send_fn(lpn_idx, CMD_CTL_CLEAR); // use normal fifo, not mesh_adv_fifo_fn2lpn_
                    proc_fn->clear_cmd_tick = proc_fn->clear_start_tick = clock_time_100ms();
                    proc_fn->clear_int_100ms = FRI_FIRST_CLR_INTERVAL_100MS;
                    proc_fn->clear_poll = 1;
                }
            }
        }else if(DELAY_SUBSC_LIST == p_delay->delay_type){
            friend_cmd_send_subsc_conf(p_delay->adr_dst, (u8)p_delay->par_val);
        }else if(DELAY_CLEAR_CONF == p_delay->delay_type){
            mesh_ctl_fri_clear_t clear;
            clear.LPNAdr = fn_other_par[lpn_idx].LPNAdr;
            clear.LPNCounter = p_delay->par_val;
            use_mesh_adv_fifo_fn2lpn = 1;
            friend_cmd_send_clear_conf(clear.LPNAdr, (u8 *)&clear, sizeof(mesh_ctl_fri_clear_t));
            use_mesh_adv_fifo_fn2lpn = 0;
        }
        
        p_delay->delay_type = 0;
    }
	
	if(my_fifo_data_cnt_get(&mesh_adv_fifo_fn2lpn)){
		fn_quick_send_adv();	// "poll rsp" may be delay when in BLE_S window, so quickly send here again. and also "send_subsc_conf /send_clear_conf" need quick send.
	}

	if(print_cache_flag){
		mesh_cmd_bear_t *bear_temp = (mesh_cmd_bear_t *)p_delay->poll_rsp;
		LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)&bear_temp->len, min(26,bear_temp->len+1),"Data for poll:");
	}
}

void mesh_friend_ship_proc_FN(u8 *bear) // subscription add/remove must be unsegment, please refer to spec section of "Friendship" -> "Low Power feature".
{
	foreach(i,g_max_lpn_num){ // a friend node may establish friendship with many LPN, so check all LPN.
		mesh_fri_ship_proc_fn_t *proc_fn = &fri_ship_proc_fn[i];
	    if(!bear){
	        if(proc_fn->status){ // (FRI_ST_IDLE != proc_fn->status)
    	        if(FRI_ST_OFFER == proc_fn->status){
    	            if(clock_time_exceed(proc_fn->offer_tick, proc_fn->offer_delay*1000)){
						// send friend offer and set to state of receiving friend poll after received friend request.
    	                use_mesh_adv_fifo_fn2lpn = 1;
    	                friend_cmd_send_fn(i, CMD_CTL_OFFER);
    	                use_mesh_adv_fifo_fn2lpn = 0;
    	                proc_fn->offer_tick = clock_time()|1;
    	                mesh_friend_ship_set_st_fn(i, FRI_ST_POLL);
    	            }
    	        }else if(FRI_ST_POLL == proc_fn->status){
    	            // add 500ms, because handling response of POLL was delay some ten ms. 
    	            if(clock_time_exceed(proc_fn->offer_tick, (500+FRI_ESTABLISH_OFFER_MS)*1000)){
    	            	// timeout to receive friend poll from LPN after send friend offer to LPN, 
    	            	// means that LPN did not receive offer, or LPN did not select current FN as friend node,
    	            	// or FN did not receive the friend poll from LPN.
    	                mesh_friend_ship_proc_init_fn(i);
    	            }
    	        }else if(FRI_ST_TIMEOUT_CHECK == proc_fn->status){
    	            if(clock_time_exceed_100ms(proc_fn->poll_tick, (u32)(fn_req[i].PollTimeout))){
						// timeout to receive friend poll from LPN, then will disconnect this friendship.
    	                friend_ship_disconnect_fn(i, FS_DISCONNECT_TYPE_POLL_TIMEOUT);
    	            }
    	        }
	        }
	        	        
	        if(proc_fn->clear_poll){    // clear by other FN
	            if(clock_time_exceed_100ms(proc_fn->clear_start_tick, (u32)(fn_req[i].PollTimeout)*2)){
					// when the timeout expires, even if the clear response has not been received yet, the clear command will stop being sent.
	                mesh_stop_clear_cmd(i);
	            }else{
	                if(clock_time_exceed_100ms(proc_fn->clear_cmd_tick, proc_fn->clear_int_100ms)){
	                	// Gradually reduce the frequency of sending clear commands. 
	                	// please refer to mesh V1.1 spec "Figure 3.24: Friend Clear procedure example" of "3.6.6.3.1 Friend establishment".
	                    proc_fn->clear_cmd_tick = clock_time_100ms();
	                    proc_fn->clear_int_100ms = proc_fn->clear_int_100ms << 1;
	                    friend_cmd_send_fn(i, CMD_CTL_CLEAR);
	                }
	            }
	        }

	        if(proc_fn->clear_by_lpn_tick && clock_time_exceed(proc_fn->clear_by_lpn_tick, 5*1000*1000)){
	            // when received friend clear, should not clear at once, and need to delay some time to clear Friend ship.
	            // because LPN may retry sending friend clear command when not receive clear confirm.
                friend_ship_disconnect_fn(i, FS_DISCONNECT_TYPE_CLEAR);
	        }
	    }else{ // to process packet received
	        mesh_cmd_bear_t *p_bear = (mesh_cmd_bear_t *)bear;
	        mesh_cmd_nw_t *p_nw = &p_bear->nw;
	        mesh_cmd_lt_ctl_unseg_t *p_lt_ctl_unseg = &p_bear->lt_ctl_unseg;
			adv_report_extend_t *p_extend = get_adv_report_extend(&p_bear->len);
	        u8 op = p_lt_ctl_unseg->opcode;
	        if(!((CMD_CTL_REQUEST == op)||(CMD_CTL_CLEAR == op)||(CMD_CTL_CLR_CONF == op))
			 && !(proc_fn->status && (fn_other_par[i].LPNAdr == p_nw->src))){
	        	continue ;
	        }

	        if(CMD_CTL_REQUEST == op){
	            mesh_ctl_fri_req_t *p_req = (mesh_ctl_fri_req_t *)(p_lt_ctl_unseg->data);
				#if DEBUG_SUSPEND
	            static u8 mesh_lpn_debug_req;mesh_lpn_debug_req++;
	            static mesh_ctl_fri_req_t mesh_lpn_debug_req_buf;
	            memcpy(&mesh_lpn_debug_req_buf, p_req, sizeof(mesh_ctl_fri_req_t));
				#endif
	            if(0 == mesh_friend_request_is_valid(p_req)){
                    LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_req, sizeof(mesh_ctl_fri_req_t),"Error:rcv Invalid friend request:");
	                return ;
	            }
				
				#if DEBUG_SUSPEND
	            static u8 mesh_lpn_debug_req2;mesh_lpn_debug_req2++;
				#endif

	            if(0 == proc_fn->status){
                    mesh_friend_ship_clear_FN(i);   // just init parameters
	            }else if(p_nw->src == fn_other_par[i].LPNAdr){  // was friend before
                    friend_ship_disconnect_fn(i, FS_DISCONNECT_TYPE_RX_REQUEST);
	            }else{
					continue;
	            }
				
				#if DEBUG_SUSPEND
	            static u8 mesh_lpn_debug_req3;mesh_lpn_debug_req3++;
				#endif
	            LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_req, sizeof(mesh_ctl_fri_req_t),"rcv friend request sno:0x%x src:0x%x par:",p_nw->sno[0]|(p_nw->sno[1]<<8)|(p_nw->sno[2]<<16), p_nw->src);
	            fn_other_par[i].LPNAdr = p_nw->src;
	            fn_other_par[i].FriAdr = ele_adr_primary;
	            fn_other_par[i].p_cache = &mesh_fri_cache_fifo[i];
	            fn_other_par[i].nk_sel_dec_fn = mesh_key.netkey_sel_dec;
	            fn_other_par[i].TransNo = -1; // init to be different with the first TransNo.
	            fn_poll[i].FSN = 1;           // init to be different with the first poll FSN.
	            memcpy(fn_req+i, p_req,sizeof(mesh_ctl_fri_req_t));
	            proc_fn->offer_tick = clock_time()|1;
				fn_offer[i].RSSI = p_extend->rssi;    // rssi of request message. // check rssi to add more random delay for offer message, if not, offer delay will always be minimum value(100ms + 0~10ms).
	            proc_fn->offer_delay = mesh_friend_local_delay(i);
	            mesh_friend_ship_set_st_fn(i, FRI_ST_OFFER);
	            return ;
	        }else if(CMD_CTL_POLL == op){
	            mesh_ctl_fri_poll_t *p_poll = (mesh_ctl_fri_poll_t *)(p_lt_ctl_unseg->data);
	            if(0 == mesh_friend_poll_is_valid(p_poll)){
                    LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_poll, sizeof(mesh_ctl_fri_poll_t),"Error:rcv Invalid friend poll:");
	                return ;
	            }
	            
                if(fn_other_par[i].FriAdr != p_nw->dst){
                    continue ;
                }
				
				#if 0
                static u8 AA_discard_cnt = 0;
                if(AA_discard_cnt){
                    AA_discard_cnt --;
                    return ;
                }
				#endif
	            LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_poll, sizeof(mesh_ctl_fri_poll_t),"rcv friend poll sno:0x%x par:",p_nw->sno[0]|(p_nw->sno[1]<<8)|(p_nw->sno[2]<<16));
	            if(proc_fn->offer_tick){
					fn_other_par[i].link_ok = 1;
	                mesh_friend_ship_proc_init_fn(i);    // init parameters after establish friend ship
	                if(fn_req[i].PreAdr && !is_own_ele(fn_req[i].PreAdr)){
	                    proc_fn->clear_delay_cnt = 1;   // 1: means no delay
	                }
	                friend_ship_establish_ok_cb_fn(i);
	            }else if(proc_fn->clear_by_lpn_tick){
                    friend_ship_disconnect_fn(i, FS_DISCONNECT_TYPE_CLEAR);
                    return ;
	            }
	            
	            int poll_retry_flag = (fn_poll[i].FSN == p_poll->FSN);
				if(0 == poll_retry_flag){
					fn_poll[i].FSN = p_poll->FSN;
				}
				u8 *p_br_cache = get_cache_buf_for_poll(i, poll_retry_flag, 0);

                u32 timeStamp = p_extend->timeStamp;
                #ifndef WIN32
                if((u32)(clock_time() - timeStamp) > 100*1000*sys_tick_per_us){
                    timeStamp = clock_time();
                }
                #endif
                
	            mesh_friend_set_delay_par_poll(i, p_br_cache, timeStamp);

	            // start poll timeout check
	            mesh_friend_ship_set_st_fn(i, FRI_ST_TIMEOUT_CHECK);
	            mesh_reset_poll_timeout_timer(i);
	            return ;
	        }else if(CMD_CTL_CLEAR == op){
	        	mesh_ctl_fri_clear_t *p_fri_clear = (mesh_ctl_fri_clear_t *)p_lt_ctl_unseg->data;
	            if((fn_other_par[i].LPNAdr == p_fri_clear->LPNAdr)
	            && (p_fri_clear->LPNCounter - fn_req[i].LPNCounter <= 255)){
	            	LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_fri_clear, sizeof(mesh_ctl_fri_clear_t),"rcv friend clear:");
	            	if(fn_other_par[i].LPNAdr == p_nw->src){
                        mesh_friend_set_delay_par(DELAY_CLEAR_CONF, i, p_nw->src, p_fri_clear->LPNCounter);
                        proc_fn->clear_by_lpn_tick = clock_time()|1;
					}else{
                        friend_cmd_send_clear_conf(p_nw->src, (u8 *)p_fri_clear, sizeof(mesh_ctl_fri_clear_t));
                        friend_ship_disconnect_fn(i, FS_DISCONNECT_TYPE_CLEAR);
	                }
					return ;
	            }
	        }else if(CMD_CTL_CLR_CONF == op){
	        	mesh_ctl_fri_clear_conf_t *p_clear_conf = (mesh_ctl_fri_clear_conf_t *)p_lt_ctl_unseg->data;
	            if(proc_fn->clear_poll && (fn_other_par[i].LPNAdr == p_clear_conf->LPNAdr)){
					LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_clear_conf, sizeof(mesh_ctl_fri_clear_conf_t),"rcv friend clear confirm:");
	                mesh_stop_clear_cmd(i);
					return ;
	            }
	        }else if((CMD_CTL_SUBS_LIST_ADD == op) || (CMD_CTL_SUBS_LIST_REMOVE == op)){
				/* subscription add/remove must be unsegment, please refer to spec section of "Friendship" -> "Low Power feature":
				   All transport control messages originated by a Low Power node shall be sent as Unsegmented Control
				   messages with the SRC field set to the unicast address of the primary element of the node that supports
				   the Low Power feature.
				*/
				mesh_ctl_fri_subsc_list_t *p_subsc = CONTAINER_OF(p_lt_ctl_unseg->data,mesh_ctl_fri_subsc_list_t,TransNo);
	            mesh_reset_poll_timeout_timer(i);

	            //if(fn_other_par[i].TransNo != p_subsc->TransNo){
	                u32 subsc_cnt = mesh_subsc_adr_cnt_get(p_bear);
	                u16 adr[SUB_LIST_MAX_IN_ONE_MSG];
	                memcpy(adr, p_subsc->adr, sizeof(adr));
	                if(CMD_CTL_SUBS_LIST_ADD == op){
						LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_subsc->adr, subsc_cnt*2,"rcv friend sub list add:");
	                    friend_subsc_list_add_adr((lpn_adr_list_t *)(&fn_other_par[i].SubsList), (lpn_adr_list_t *)adr, subsc_cnt);
	                }else{  // (CMD_CTL_SUBS_LIST_REMOVE == op)
	                	LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)p_subsc->adr, subsc_cnt*2,"rcv friend sub list remove:");
	                    friend_subsc_list_rmv_adr((lpn_adr_list_t *)(&fn_other_par[i].SubsList), (lpn_adr_list_t *)adr, subsc_cnt);
	                }
					//LOG_MSG_LIB(TL_LOG_FRIEND,(u8 *)&fn_other_par[i].SubsList, sizeof(fn_other_par[i].SubsList),"LPN sublist:");
	            //}
	            mesh_friend_set_delay_par(DELAY_SUBSC_LIST, i, p_nw->src, p_subsc->TransNo);
	            fn_other_par[i].TransNo = p_subsc->TransNo;
	        }else{
                LOG_MSG_LIB(TL_LOG_FRIEND,0, 0,"Error:rcv Invalid op code %2x:",op);
	        }
	    }
	}
}







u8 g_max_lpn_num = MAX_LPN_NUM;

#if 1 // only friend node use
STATIC_ASSERT(MAX_LPN_NUM <= 16);	// because F2L is 16bit

// mesh_friend_seg_cache_t friend_seg_cache;
u8 mesh_fri_cache_fifo_b[MAX_LPN_NUM][(sizeof(mesh_cmd_bear_t)+DELTA_EXTEND_AND_NORMAL_ALIGN4_BUF) * (1 << FN_CACHE_SIZE_LOG)]={{0}};
my_fifo_t mesh_fri_cache_fifo[MAX_LPN_NUM] = {};//{,,0,0, name##_b};

mesh_ctl_fri_poll_t fn_poll[MAX_LPN_NUM] = {};
mesh_ctl_fri_update_t fn_update[MAX_LPN_NUM] = {};
mesh_ctl_fri_req_t fn_req[MAX_LPN_NUM] = {};
mesh_ctl_fri_offer_t fn_offer[MAX_LPN_NUM] = {};
mesh_fri_ship_other_t fn_other_par[MAX_LPN_NUM] = {};
fn_ctl_rsp_delay_t fn_ctl_rsp_delay[MAX_LPN_NUM] = {};

mesh_fri_ship_proc_fn_t fri_ship_proc_fn[MAX_LPN_NUM] = {};

MYFIFO_INIT(mesh_adv_fifo_fn2lpn, (sizeof(mesh_cmd_bear_t)+DELTA_EXTEND_AND_NORMAL_ALIGN4_BUF), 4);

#endif

void mesh_feature_set_fn(){
    #if FRI_SAMPLE_EN
	friend_ship_sample_message_test();
    #else
	#if (FEATURE_FRIEND_EN)
	foreach(i, MAX_LPN_NUM){
	    fn_other_par[i].FriAdr = ele_adr_primary;

	    mesh_ctl_fri_update_t *p_update = fn_update+i;
	    get_iv_big_endian(p_update->IVIndex, (u8 *)&iv_idx_st.iv_tx);

	    mesh_ctl_fri_offer_t *p_offer = (fn_offer+i);
	    #if 0
	    if(ele_adr_primary == ADR_FND2){
	        p_offer->RecWin = 0xfa;
	        p_offer->CacheSize = 0x02;
	        p_offer->SubsListSize = 0x05;
	        p_offer->RSSI = -90;    // 0xa6
	        p_offer->FriCounter = 0x000a - 1;  // increase counter later
	    }else
	    #endif
	    {
	        p_offer->RecWin = FRI_REC_WIN_MS; // 200;// 
	        p_offer->CacheSize = 1 << FN_CACHE_SIZE_LOG;
	        p_offer->SubsListSize = SUB_LIST_MAX_LPN;
	        p_offer->RSSI = FN_RSSI_INVALID; // -70;    // 0xba
	        p_offer->FriCounter = 0x072f - 1;  // increase counter later
	    }
	}
	#endif
    #endif
}

#if 0
void fn_update_RecWin(u8 RecWin)
{
#if 0   // no stop friend ship should be better.
    if(!is_fn_support_and_en){
        return;
    }
    
	foreach(i, MAX_LPN_NUM){
	    mesh_ctl_fri_offer_t *p_offer = (fn_offer+i);
	    if(p_offer->RecWin != RecWin){
            p_offer->RecWin = RecWin;
            if(fn_other_par[i].link_ok){
                mesh_friend_ship_proc_init_fn(i);    // stop friend ship
            }
        }
    }
#endif
}

u32 get_RecWin_connected()
{
    return ((get_blt_conn_interval_us()+999)/1000 + 10);
}
#endif

u8 mesh_get_fn_cache_size_log_cnt()
{
    return (1<<FN_CACHE_SIZE_LOG);
}

/**
* friend node: when friend ship establish ok, this function would be called.
* @params: lpn_idx: array index of fn_other_par[].
*/
void friend_ship_establish_ok_cb_fn(u8 lpn_idx)
{

}

/**
* friend node: when friend ship disconnect, this function would be called.
* @params: lpn_idx: array index of fn_other_par[].
* @params: type: disconnect type.for example FS_DISCONNECT_TYPE_POLL_TIMEOUT.
*/
void friend_ship_disconnect_cb_fn(u8 lpn_idx, int type)
{

}


void mesh_global_var_init_fn_buf()
{
	foreach(i,MAX_LPN_NUM){
		mesh_fri_cache_fifo[i].size = (sizeof(mesh_fri_cache_fifo_b[0])/(1 << FN_CACHE_SIZE_LOG));
		mesh_fri_cache_fifo[i].num = (1 << FN_CACHE_SIZE_LOG);
		mesh_fri_cache_fifo[i].wptr = 0;
		mesh_fri_cache_fifo[i].rptr = 0;
		mesh_fri_cache_fifo[i].p = mesh_fri_cache_fifo_b[i];
	}
}
#else
    #if (0 == FEATURE_LOWPOWER_EN)
u8 g_max_lpn_num = 0;   // must 0
mesh_fri_ship_other_t fn_other_par[MAX_LPN_NUM];// = {0};
    #endif

void mesh_iv_update_start_poll_fn(u8 iv_update_by_sno, u8 beacon_iv_update_pkt_flag){}
u16 mesh_group_match_friend(u16 adr){return 0;}
int mesh_tx_cmd_add_packet_fn2lpn(u8 *p_bear){return -1;}
void mesh_friend_ship_init_all(){}
u32 get_poll_timeout_fn(u16 lpn_adr){return 0;}
u16 mesh_mac_match_friend(u16 adr){return 0;}
mesh_fri_ship_other_t * mesh_fri_cmd2cache(u8 *p_bear_big, u8 len_nw, u8 adv_type, u8 trans_par_val, u16 F2L_bit){return 0;}
int is_unicast_friend_msg_to_lpn(mesh_cmd_nw_t *p_nw){return 0;}
void mesh_rc_segment_handle_fn(mesh_match_type_t *p_match_type, mesh_cmd_nw_t *p_nw){}
void mesh_friend_ship_proc_FN(u8 *bear){}
int friend_cache_check_replace(u8 lpn_idx, mesh_cmd_bear_t *bear_big){return 0;}
u8 get_tx_nk_arr_idx_friend(u16 adr, u16 op){return 0;}
    #if (0 == FEATURE_LOWPOWER_EN)
void mesh_friend_logmsg(mesh_cmd_bear_t *p_bear_big, u8 len){}
    #endif
int is_unicast_friend_msg_from_lpn(mesh_cmd_nw_t *p_nw){return 0;}
u32 get_current_poll_timeout_timer_fn(u16 lpn_adr){return 0;};

#endif

