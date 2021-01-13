/*
Project :
1. ESP32 Fan Control with Temperature
2. ESP32 Buzzer Alarm with Time
3. ESP32 LED Control with Bluetooth

Author      : Ujang Supriyadi
Email       : ujang.supriyadi568@gmail.com
Last Edited : 11/01/2021
*/

#include "header.h"
#include "bluetooth.c"
#include "wifi.c"
#include "initial.c"

static void sensor_process(void)
{
    val_opamp = adc1_get_raw(ADC1_CHANNEL_0);
    val_lm = val_opamp / 2;
    mv_lm = (val_lm / 4096.0) * 1100;
    mv_opamp = (val_opamp / 4096.0) * 1100;
    cel = mv_lm / 10;
}

static void set_bt(void)
{
    if(!strcmp(text,"1"))
    {
        gpio_set_level(LED,1);
        strcpy(lamp, "ON");
    }
    else if(!strcmp(text,"2"))
    {
        gpio_set_level(LED, 0);
        strcpy(lamp, "OFF");
    }
    else if(!strcmp(text,"3"))
    {
        if (EN)
            Alarm = 1;
    }
    else if(!strcmp(text,"4"))
    {
       Alarm = 0;
    }
    else if(!strcmp(text,"5"))
    {
        if (!EN)
            EN = 1;
        else
        {
            EN = 0;
            Alarm = 0;
        }
    }
    else if (!strcmp(text,"6"))
        pesan = set_time;

    if ((!strcmp(selection,"::")) && (strlen(text) == 8))
        strcpy(set_time, text);

}

static void SendData(void)
{
    sprintf(request,"%s?&ADC=%d&LM741=%.2f&LM35=%.2f&Temperature=%.2f&StatusLED='%s'",URL,val_opamp,mv_opamp,mv_lm,cel,led);
    printf("%s\n", request);
    trigger_http_request(request); 
}

static void cetak(void)
{
    printf("Time  -->  %s\n", strftime_buf);
    printf("LM35  -->  Raw : %d   Voltage : %.2f mv\n", val_lm, mv_lm);
    printf("LM741 -->  Raw : %d   Voltage : %.2f mv\n", val_opamp, mv_opamp);
    printf("Temp  -->  %.2f°C    Speed   : %.2f\n", cel, speed);
    printf("Alarm -->  %s  (%d)\n", set_time, Alarm);
    printf("Lamp  -->  %s\n\n", lamp);
}

static void run_program(void)
{
    set_pwm();

    while (1)
    {
        get_time();
        sensor_process();

        if (cel > 37)
        {
            gpio_set_level(LED_RED, 1);
            gpio_set_level(LED_GREEN, 0);
            gpio_set_level(LED_BLUE, 0);
            strcpy(led, "Red");

            speed = 100.0;
        }
        else if (cel >= 30 && cel <= 37)
        {
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 1);
            gpio_set_level(LED_BLUE, 0);
            strcpy(led, "Green");

            speed = mappingValue(cel, 30.0, 37.0, 50.0, 90.0);
        }
        else if (cel < 30)
        {
            gpio_set_level(LED_RED, 0);
            gpio_set_level(LED_GREEN, 0);
            gpio_set_level(LED_BLUE, 1);
            strcpy(led, "Blue");

            if (cel > 25)
                speed = mappingValue(cel, 25.0, 30.0, 40.0, 50.0);
            else
                speed = 0.0;
        }

        FanActived(MCPWM_UNIT_0, MCPWM_TIMER_0, speed);
        cetak();

        if (!(strcmp(strftime_sec, "00")))
            SendData();

        if (EN)
        {
            if (!(strcmp(strftime_set, set_time)))
                Alarm = 1;

            if (Alarm)
            {
                SetAlarm(BUZZER, 1);
                counter++;
                if (counter >= 60)
                {
                    counter = 0;
                    Alarm = 0;
                }
            }
            else
            {
                counter = 0;
                SetAlarm(BUZZER, 0);
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
        else
        {
            Alarm = 0;
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    set_configuration();
    startClassicBtSpp();
    
    initialise_wifi();
    initialize_sntp();

    run_program();
}