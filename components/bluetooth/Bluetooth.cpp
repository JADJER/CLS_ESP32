// Copyright 2023 Pavel Suprunov
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by jadjer on 14.02.23.
//

#include "bluetooth/Bluetooth.hpp"

#include <nvs.h>
#include <nvs_flash.h>

#include "bluetooth/blehr_sens.hpp"

/* BLE */
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "console/console.h"
#include "services/gap/ble_svc_gap.h"

constexpr auto tag = "Bluetooth";

constexpr auto device_name = "blehr_sensor_1.0";

static bool notify_state;
static uint16_t conn_handle;
static uint8_t blehr_addr_type;
static uint8_t heartrate = 90;

Bluetooth::Bluetooth() {
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    int rc;

    nimble_port_init();
    /* Initialize the NimBLE host configuration */
    ble_hs_cfg.sync_cb = blehrOnSync;
    ble_hs_cfg.reset_cb = blehrOnReset;

    rc = gatt_svr_init();
    assert(rc == 0);

    /* Set the default device name */
    rc = ble_svc_gap_device_name_set(device_name);
    assert(rc == 0);

    /* Start the task */
//    nimble_port_freertos_init(blehrHostTask);

    nimble_port_run();
}


Bluetooth::~Bluetooth() {
    nimble_port_freertos_deinit();
}

void Bluetooth::spinOnce() {
    blehrTxHrate();
}

void Bluetooth::printAddr(const uint8_t *addr) {
    ESP_LOGI(tag, "%02x:%02x:%02x:%02x:%02x:%02x", addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
}

void Bluetooth::blehrOnSync() {
    int rc;

    rc = ble_hs_id_infer_auto(0, &blehr_addr_type);
    assert(rc == 0);

    uint8_t addr_val[6] = {0};
    rc = ble_hs_id_copy_addr(blehr_addr_type, addr_val, nullptr);

    ESP_LOGI(tag, "Device Address:");
    printAddr(addr_val);

    /* Begin advertising */
    blehrAdvertise();
}

void Bluetooth::blehrOnReset(int reason) {
    ESP_LOGE(tag, "Resetting state; reason=%d", reason);
}

void Bluetooth::blehrAdvertise() {
    ble_gap_adv_params adv_params{};
    ble_hs_adv_fields fields{};
    int rc;

    /*
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info)
     *     o Advertising tx power
     *     o Device name
     */
    memset(&fields, 0, sizeof(fields));

    /*
     * Advertise two flags:
     *      o Discoverability in forthcoming advertisement (general)
     *      o BLE-only (BR/EDR unsupported)
     */
    fields.flags = BLE_HS_ADV_F_DISC_GEN |
                   BLE_HS_ADV_F_BREDR_UNSUP;

    /*
     * Indicate that the TX power level field should be included; have the
     * stack fill this value automatically.  This is done by assigning the
     * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
     */
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;

    fields.name = (uint8_t *) device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        MODLOG_DFLT(ERROR, "error setting advertisement data; rc=%d\n", rc);
        return;
    }

    /* Begin advertising */
    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;
    rc = ble_gap_adv_start(blehr_addr_type, nullptr, BLE_HS_FOREVER, &adv_params, blehrGapEvent, nullptr);
    if (rc != 0) {
        MODLOG_DFLT(ERROR, "error enabling advertisement; rc=%d\n", rc);
        return;
    }
}

void Bluetooth::blehrTxHrate() {
    static uint8_t hrm[2];
    int rc;
    struct os_mbuf *om;

    if (!notify_state) {
        blehrTxHrateStop();
        heartrate = 90;
        return;
    }

    hrm[0] = 0x06; /* contact of a sensor */
    hrm[1] = heartrate; /* storing dummy data */

    /* Simulation of heart beats */
    heartrate++;
    if (heartrate == 160) {
        heartrate = 90;
    }

    om = ble_hs_mbuf_from_flat(hrm, sizeof(hrm));
    rc = ble_gattc_notify_custom(conn_handle, hrs_hrm_handle, om);

    assert(rc == 0);

    blehrTxHrateReset();
}

int Bluetooth::blehrGapEvent(struct ble_gap_event *event, void *arg) {
    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            /* A new connection was established or a connection attempt failed */
            ESP_LOGI(tag, "connection %s; status=%d", event->connect.status == 0 ? "established" : "failed", event->connect.status);

            if (event->connect.status != 0) {
                /* Connection failed; resume advertising */
                blehrAdvertise();
            }

            conn_handle = event->connect.conn_handle;

            break;

        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI(tag, "disconnect; reason=%d", event->disconnect.reason);

            /* Connection terminated; resume advertising */
            blehrAdvertise();
            break;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            ESP_LOGI(tag, "adv complete");
            blehrAdvertise();
            break;

        case BLE_GAP_EVENT_SUBSCRIBE:
            ESP_LOGI(tag, "subscribe event; cur_notify=%d\n value handle; val_handle=%d", event->subscribe.cur_notify, hrs_hrm_handle);
            if (event->subscribe.attr_handle == hrs_hrm_handle) {
                notify_state = event->subscribe.cur_notify;
                blehrTxHrateReset();
            } else if (event->subscribe.attr_handle != hrs_hrm_handle) {
                notify_state = event->subscribe.cur_notify;
                blehrTxHrateStop();
            }
            ESP_LOGI("BLE_GAP_SUBSCRIBE_EVENT", "conn_handle from subscribe=%d", conn_handle);
            break;

        case BLE_GAP_EVENT_MTU:
            ESP_LOGI(tag, "mtu update event; conn_handle=%d mtu=%d", event->mtu.conn_handle, event->mtu.value);
            break;

    }

    return 0;
}

void Bluetooth::blehrHostTask(void *param) {
    ESP_LOGI(tag, "BLE Host Task Started");
    /* This function will return only when nimble_port_stop() is executed */
    nimble_port_run();

    nimble_port_freertos_deinit();
}
