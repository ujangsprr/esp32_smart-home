#include "header.h"

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    ESP_LOGI(SPP_TAG, "Start of: static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)");
    switch (event)
    {
    case ESP_SPP_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_INIT_EVT");
        ESP_LOGI(SPP_TAG, "Call esp_bt_dev_set_device_name(EXAMPLE_DEVICE_NAME)");
        esp_bt_dev_set_device_name(DEVICE_NAME);
        ESP_LOGI(SPP_TAG, "Call esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE)");
        esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
        ESP_LOGI(SPP_TAG, "Call esp_spp_start_srv(sec_mask,role_slave, 0, SPP_SERVER_NAME)");
        esp_spp_start_srv(sec_mask, role_slave, 0, SPP_SERVER_NAME);
        break;
    case ESP_SPP_DISCOVERY_COMP_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DISCOVERY_COMP_EVT");
        break;
    case ESP_SPP_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_OPEN_EVT");

        ESP_LOGI(SPP_TAG, "bWriteAfterOpenEvt: %d: ", bWriteAfterOpenEvt);
        if (bWriteAfterOpenEvt)
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterOpenEvt = true");
            ESP_LOGI(SPP_TAG, "Call esp_spp_write(param->srv_open.handle, SPP_DATA_LEN, spp_data)");

            esp_spp_write(param->srv_open.handle, SPP_DATA_LEN, spp_data);

            char *c = set_time;
            uint8_t *u = (uint8_t *)c;
            esp_spp_write(param->srv_open.handle, strlen(c), u);
        }
        else
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterOpenEvt = false");
        }
        break;
    case ESP_SPP_CLOSE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CLOSE_EVT");
        break;
    case ESP_SPP_START_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_START_EVT");
        break;
    case ESP_SPP_CL_INIT_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CL_INIT_EVT");
        break;
    case ESP_SPP_DATA_IND_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_DATA_IND_EVT len=%d handle=%d", param->data_ind.len, param->data_ind.handle);
        ESP_LOGI(SPP_TAG, "Call esp_log_buffer_hex("
                          ",param->data_ind.data,param->data_ind.len)");

        esp_log_buffer_hex("Received HEX Data", param->data_ind.data, param->data_ind.len);
        esp_log_buffer_char("Received String Data", param->data_ind.data, param->data_ind.len);

        if (bWriteAfterDataReceived)
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterDataReceived = true");

            for (int i = 0; i < param->data_ind.len; ++i)
                text[i] = (char)param->data_ind.data[i];

            selection[0] = text[2];
            selection[1] = text[5];

            printf("%s", (char *)text);
            printf("\n");

            set_bt();

            memset(text,0,strlen(text));
        }
        else
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterDataReceived = false");
        }
        
        esp_spp_write(param->srv_open.handle, 8, (uint8_t *)pesan);

        break;
    case ESP_SPP_CONG_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_CONG_EVT");
        break;
    case ESP_SPP_WRITE_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_WRITE_EVT len=%d cong=%d", param->write.len, param->write.cong);
        esp_log_buffer_hex("HEX Data was sent", spp_data, SPP_DATA_LEN);

        ESP_LOGI(SPP_TAG, "if param->write.cong ...");
        if (param->write.cong == 0)
        {
            ESP_LOGI(SPP_TAG, "param->write.cong == 0");
            if (bWriteAfterWriteEvt)
            {
                ESP_LOGI(SPP_TAG, "bWriteAfterWriteEvt = true");
                ESP_LOGI(SPP_TAG, "Call esp_spp_write(param->write.handle, SPP_DATA_LEN, spp_data)");
                esp_spp_write(param->write.handle, SPP_DATA_LEN, spp_data);
            }
            else
            {
                ESP_LOGI(SPP_TAG, "bWriteAfterWriteEvt = false");
            }
        }
        else
        {
            ESP_LOGI(SPP_TAG, "param->write.cong <> 0");
        }
        break;
    case ESP_SPP_SRV_OPEN_EVT:
        ESP_LOGI(SPP_TAG, "ESP_SPP_SRV_OPEN_EVT");

        ESP_LOGI(SPP_TAG, "bWriteAfterOpenEvt: %d: ", bWriteAfterSvrOpenEvt);

        if (bWriteAfterSvrOpenEvt)
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterSvrOpenEvt = true");

            char *c = set_time;
            uint8_t *u = (uint8_t *)c;
            ESP_LOGI(SPP_TAG, "Call esp_spp_write(param->srv_open.handle, 5, Hello)");
            esp_spp_write(param->srv_open.handle, strlen(c), u);
        }
        else
        {
            ESP_LOGI(SPP_TAG, "bWriteAfterSvrOpenEvt = false");
        }
        break;
    default:
        break;
    }
    ESP_LOGI(SPP_TAG, "End of: static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)");
}

void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    ESP_LOGI(SPP_TAG, "Start of: void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)");
    switch (event)
    {
    case ESP_BT_GAP_AUTH_CMPL_EVT:
    {
        if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGI(SPP_TAG, "authentication success: %s", param->auth_cmpl.device_name);
            esp_log_buffer_hex(SPP_TAG, param->auth_cmpl.bda, ESP_BD_ADDR_LEN);
        }
        else
        {
            ESP_LOGE(SPP_TAG, "authentication failed, status:%d", param->auth_cmpl.stat);
        }
        break;
    }
    case ESP_BT_GAP_PIN_REQ_EVT:
    {
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_PIN_REQ_EVT min_16_digit:%d", param->pin_req.min_16_digit);
        if (param->pin_req.min_16_digit)
        {
            ESP_LOGI(SPP_TAG, "Input pin code: 0000 0000 0000 0000");
            esp_bt_pin_code_t pin_code = {0};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 16, pin_code);
        }
        else
        {
            ESP_LOGI(SPP_TAG, "Input pin code: 1234");
            esp_bt_pin_code_t pin_code;
            pin_code[0] = '1';
            pin_code[1] = '2';
            pin_code[2] = '3';
            pin_code[3] = '4';
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin_code);
        }
        break;
    }

    case ESP_BT_GAP_CFM_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_CFM_REQ_EVT Please compare the numeric value: %d", param->cfm_req.num_val);
        esp_bt_gap_ssp_confirm_reply(param->cfm_req.bda, true);
        break;
    case ESP_BT_GAP_KEY_NOTIF_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_NOTIF_EVT passkey:%d", param->key_notif.passkey);
        break;
    case ESP_BT_GAP_KEY_REQ_EVT:
        ESP_LOGI(SPP_TAG, "ESP_BT_GAP_KEY_REQ_EVT Please enter passkey!");
        break;

    default:
    {
        ESP_LOGI(SPP_TAG, "event: %d", event);
        break;
    }
    }
    return;
}

void startClassicBtSpp(void)
{
    for (int i = 0; i < SPP_DATA_LEN; ++i)
    {
        spp_data[i] = i + 65;
    }

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_LOGI(SPP_TAG, "Call esp_bt_controller_init(&bt_cfg)");
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s initialize controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "Initialize controller ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)");
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s enable controller failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "Enable controller ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_bluedroid_init()");
    if ((ret = esp_bluedroid_init()) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s initialize bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "Initialize bluedroid ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_bluedroid_enable()");
    if ((ret = esp_bluedroid_enable()) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s enable bluedroid failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "Enable bluedroid ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_bt_gap_register_callback(esp_bt_gap_cb)");
    if ((ret = esp_bt_gap_register_callback(esp_bt_gap_cb)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s gap register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "Gap register ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_spp_register_callback(esp_spp_cb)");
    if ((ret = esp_spp_register_callback(esp_spp_cb)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s spp register failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "spp register ok");
    }

    ESP_LOGI(SPP_TAG, "Call esp_spp_init(esp_spp_mode)");
    if ((ret = esp_spp_init(esp_spp_mode)) != ESP_OK)
    {
        ESP_LOGE(SPP_TAG, "%s spp init failed: %s\n", __func__, esp_err_to_name(ret));
        return;
    }
    else
    {
        ESP_LOGI(SPP_TAG, "spp init ok");
    }

    ESP_LOGI(SPP_TAG, "CONFIG_BT_SSP_ENABLED == true");

    ESP_LOGI(SPP_TAG, "Set default parameters for Secure Simple Pairing");
    esp_bt_sp_param_t param_type = ESP_BT_SP_IOCAP_MODE;
    esp_bt_io_cap_t iocap = ESP_BT_IO_CAP_IO;
    esp_bt_gap_set_security_param(param_type, &iocap, sizeof(uint8_t));

    ESP_LOGI(SPP_TAG, "Set default parameters");
    esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
    esp_bt_pin_code_t pin_code;
    esp_bt_gap_set_pin(pin_type, 0, pin_code);
}