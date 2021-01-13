#ifndef HEADER_
#define HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_event_loop.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "lwip/err.h"
#include "esp_sntp.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "time.h"
#include "sys/time.h"
#include "esp_attr.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_adc_cal.h"

#define LED_RED 18
#define LED_GREEN 19
#define LED_BLUE 21
#define LED 3
#define BUZZER 5
#define GPIO_PWM0A_OUT 15

#define WIFI_SSID "Smart Home"
#define WIFI_PASS "12345678"
#define DEFAULT_TIME "07:00:00"
#define URL "http://192.168.43.181/monitoring/connect.php"

#define SPP_TAG "SPP_ACCEPTOR"
#define SPP_SERVER_NAME "SPP_SERVER"
#define DEVICE_NAME "Smart Home"

#define SPP_SHOW_DATA 1
#define SPP_SHOW_SPEED 1
#define SPP_SHOW_MODE SPP_SHOW_SPEED
#define SPP_DATA_LEN 20

static uint8_t spp_data[SPP_DATA_LEN];
static bool bWriteAfterOpenEvt = true;
static bool bWriteAfterWriteEvt = false;
static bool bWriteAfterSvrOpenEvt = true;
static bool bWriteAfterDataReceived = true;

static const esp_spp_mode_t esp_spp_mode = ESP_SPP_MODE_CB;
static const esp_spp_sec_t sec_mask = ESP_SPP_SEC_AUTHENTICATE;
static const esp_spp_role_t role_slave = ESP_SPP_ROLE_SLAVE;

static void initialize_sntp(void);
static void initialise_wifi(void);
static esp_err_t event_handler(void *ctx, system_event_t *event);
esp_err_t _http_event_handler(esp_http_client_event_t *evt);
static void trigger_http_request(const char *url);
static void get_time(void);
static void set_pwm(void);
static void set_configuration(void);

static void sensor_process(void);
static void run_program(void);
static void cetak(void);
static void SetAlarm(int pin, bool on);
static int mappingValue(float x, float in_min, float in_max, int out_min, int out_max);
static void set_bt(void);
static void SendData(void);

static void esp_spp_cb(esp_spp_cb_event_t event, esp_spp_cb_param_t *param);
void esp_bt_gap_cb(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param);
void startClassicBtSpp(void);

static EventGroupHandle_t wifi_event_group;
const int CONNECTED_BIT = BIT0;
static const char *TAG = "My Project";

static char strftime_buf[32], strftime_set[32], strftime_sec[32];
static char target[48], led[6], request[300];
int val_lm, val_opamp, counter = 0;
float mv_lm, mv_opamp, cel, speed;
bool Alarm = 0, On, EN = 1;
char *pesan = "";
char text[20], set_time[9] = DEFAULT_TIME, selection[5], lamp[5] = "OFF";

#endif