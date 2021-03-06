/*
 * Copyright (c) 2016-2017 The Linux Foundation. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for
 * any purpose with or without fee is hereby granted, provided that the
 * above copyright notice and this permission notice appear in all
 * copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE
 * AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#if !defined WLAN_HDD_TSF_H
#define WLAN_HDD_TSF_H

#include "wlan_hdd_cfg.h"

/**
 * enum hdd_tsf_get_state - status of get tsf action
 * @TSF_RETURN:                   get tsf
 * @TSF_STA_NOT_CONNECTED_NO_TSF: sta not connected to ap
 * @TSF_NOT_RETURNED_BY_FW:       fw not returned tsf
 * @TSF_CURRENT_IN_CAP_STATE:     driver in capture state
 * @TSF_CAPTURE_FAIL:             capture fail
 * @TSF_GET_FAIL:                 get fail
 * @TSF_RESET_GPIO_FAIL:          GPIO reset fail
 * @TSF_SAP_NOT_STARTED_NO_TSF    SAP not started
 * @TSF_NOT_READY: TSF module is not initialized or init failed
 * @TSF_DISABLED_BY_TSFPLUS: cap_tsf/get_tsf are disabled due to TSF_PLUS
 */
enum hdd_tsf_get_state {
	TSF_RETURN = 0,
	TSF_STA_NOT_CONNECTED_NO_TSF,
	TSF_NOT_RETURNED_BY_FW,
	TSF_CURRENT_IN_CAP_STATE,
	TSF_CAPTURE_FAIL,
	TSF_GET_FAIL,
	TSF_RESET_GPIO_FAIL,
	TSF_SAP_NOT_STARTED_NO_TSF,
	TSF_NOT_READY,
	TSF_DISABLED_BY_TSFPLUS
};

/**
 * enum hdd_tsf_capture_state - status of capture
 * @TSF_IDLE:      idle
 * @TSF_CAP_STATE: current is in capture state
 */
enum hdd_tsf_capture_state {
	TSF_IDLE = 0,
	TSF_CAP_STATE
};

#ifdef WLAN_FEATURE_TSF
/**
 * wlan_hdd_tsf_init() - set gpio and callbacks for
 *     capturing tsf and init tsf_plus
 * @hdd_ctx: pointer to the hdd_context_t
 *
 * This function set the callback to sme module, the callback will be
 * called when a tsf event is reported by firmware; set gpio number
 * to FW, FW will toggle this gpio when received a CAP_TSF command;
 * do tsf_plus init
 *
 * Return: nothing
 */
void wlan_hdd_tsf_init(struct hdd_context_s *hdd_ctx);

/**
 * wlan_hdd_tsf_deinit() - reset callbacks for capturing tsf, deinit tsf_plus
 * @hdd_ctx: pointer to the hdd_context_t
 *
 * This function reset the callback to sme module, and deinit tsf_plus
 *
 * Return: nothing
 */
void wlan_hdd_tsf_deinit(hdd_context_t *hdd_ctx);

/**
 * hdd_capture_tsf() - capture tsf
 * @adapter: pointer to adapter
 * @buf: pointer to uplayer buf
 * @len : the length of buf
 *
 * This function returns tsf value to uplayer.
 *
 * Return: 0 for success or non-zero negative failure code
 */
int hdd_capture_tsf(struct hdd_adapter_s *adapter, uint32_t *buf, int len);

/**
 * hdd_indicate_tsf() - return tsf to uplayer
 *
 * @adapter: pointer to adapter
 * @buf: pointer to uplayer buf
 * @len : the length of buf
 *
 * This function returns tsf value to uplayer.
 *
 * Return: Describe the execute result of this routine
 */
int hdd_indicate_tsf(struct hdd_adapter_s *adapter, uint32_t *buf, int len);

/**
 * wlan_hdd_cfg80211_handle_tsf_cmd(): Setup TSF operations
 * @wiphy: Pointer to wireless phy
 * @wdev: Pointer to wireless device
 * @data: Pointer to data
 * @data_len: Data length
 *
 * Handle TSF SET / GET operation from userspace
 *
 * Return: 0 on success, negative errno on failure
 */
int wlan_hdd_cfg80211_handle_tsf_cmd(struct wiphy *wiphy,
					struct wireless_dev *wdev,
					const void *data,
					int data_len);

int hdd_get_tsf_cb(void *pcb_cxt, struct stsf *ptsf);
#else
static inline void wlan_hdd_tsf_init(struct hdd_context_s *hdd_ctx)
{
}

static inline void wlan_hdd_tsf_deinit(hdd_context_t *hdd_ctx)
{
}

static inline int hdd_indicate_tsf(struct hdd_adapter_s *adapter, uint32_t *buf,
				int len)
{
	return -ENOTSUPP;
}

static inline int
hdd_capture_tsf(struct hdd_adapter_s *adapter, uint32_t *buf, int len)
{
	return -ENOTSUPP;
}

static inline int wlan_hdd_cfg80211_handle_tsf_cmd(struct wiphy *wiphy,
					struct wireless_dev *wdev,
					const void *data,
					int data_len)
{
	return -ENOTSUPP;
}
static inline int hdd_get_tsf_cb(void *pcb_cxt, struct stsf *ptsf)
{
	return -ENOTSUPP;
}

#endif

#if defined(WLAN_FEATURE_TSF_PLUS) && defined(WLAN_FEATURE_TSF)
#define HDD_TSF_IS_PTP_ENABLED(hdd) \
({ \
	hdd_context_t *hdd_ctx = (hdd); \
	hdd_ctx && hdd_ctx->config && hdd_ctx->config->tsf_ptp_options; \
})

#define HDD_TSF_IS_TX_SET(hdd) \
({ \
	hdd_context_t *hdd_ctx = (hdd); \
	hdd_ctx && hdd_ctx->config && \
	(hdd_ctx->config->tsf_ptp_options & CFG_SET_TSF_PTP_OPT_TX); \
})

#define HDD_TSF_IS_RX_SET(hdd) \
({ \
	hdd_context_t *hdd_ctx = (hdd); \
	hdd_ctx && hdd_ctx->config && \
	(hdd_ctx->config->tsf_ptp_options & CFG_SET_TSF_PTP_OPT_RX); \
})

#define HDD_TSF_IS_RAW_SET(hdd) \
({ \
	hdd_context_t *hdd_ctx = (hdd); \
	hdd_ctx && hdd_ctx->config && \
	(hdd_ctx->config->tsf_ptp_options & CFG_SET_TSF_PTP_OPT_RAW); \
})

#define HDD_TSF_IS_DBG_FS_SET(hdd) \
({ \
	hdd_context_t *hdd_ctx = (hdd); \
	hdd_ctx && hdd_ctx->config && \
	(hdd_ctx->config->tsf_ptp_options & CFG_SET_TSF_DBG_FS); \
})

/**
 * hdd_start_tsf_sync() - start tsf sync
 * @adapter: pointer to adapter
 *
 * This function initialize and start TSF synchronization
 *
 * Return: Describe the execute result of this routine
 */
int hdd_start_tsf_sync(hdd_adapter_t *adapter);

/**
 * hdd_stop_tsf_sync() - stop tsf sync
 * @adapter: pointer to adapter
 *
 * This function stop and de-initialize TSF synchronization
 *
 * Return: Describe the execute result of this routine
 */
int hdd_stop_tsf_sync(hdd_adapter_t *adapter);

/**
 * hdd_tsf_notify_wlan_state_change() -
 *     notify tsf module of wlan connection state
 * @old_state: old wlan state
 * @new_state: new wlan state
 *
 * This function check the old and new connection state, determine whether
 * to start or stop tsf sync
 *
 * Return: nothing
 */
void hdd_tsf_notify_wlan_state_change(hdd_adapter_t *adapter,
				      eConnectionState old_state,
				      eConnectionState new_state);

/**
 * hdd_tx_timestamp() - time stamp TX netbuf
 *
 * @netbuf: pointer to a TX netbuf
 * @target_time: TX time for the netbuf
 *
 * This function  get corresponding host time from target time,
 * and time stamp the TX netbuf with this time
 *
 * Return: Describe the execute result of this routine
 */
int hdd_tx_timestamp(qdf_nbuf_t netbuf, uint64_t target_time);

/**
 * hdd_rx_timestamp() - time stamp RX netbuf
 *
 * @netbuf: pointer to a RX netbuf
 * @target_time: RX time for the netbuf
 *
 * This function get corresponding host time from target time,
 * and time stamp the RX netbuf with this time
 *
 * Return: Describe the execute result of this routine
 */
int hdd_rx_timestamp(qdf_nbuf_t netbuf, uint64_t target_time);
#else
static inline int hdd_start_tsf_sync(hdd_adapter_t *adapter)
{
	return -ENOTSUPP;
}

static inline int hdd_stop_tsf_sync(hdd_adapter_t *adapter)
{
	return -ENOTSUPP;
}

static inline
void hdd_tsf_notify_wlan_state_change(hdd_adapter_t *adapter,
				      eConnectionState old_state,
				      eConnectionState new_state)

{
}

static inline
int hdd_tx_timestamp(qdf_nbuf_t netbuf, uint64_t target_time)
{
	return -ENOTSUPP;
}

static inline
int hdd_rx_timestamp(qdf_nbuf_t netbuf, uint64_t target_time)
{
	return -ENOTSUPP;
}
#endif

#endif
