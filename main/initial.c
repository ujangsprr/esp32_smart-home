#include "header.h"

static void mcpwm_gpio_initialize(void)
{
    printf("initializing mcpwm gpio...\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, GPIO_PWM0A_OUT);
}

static void FanActived(mcpwm_unit_t mcpwm_num, mcpwm_timer_t timer_num, float duty_cycle)
{
    mcpwm_set_duty(mcpwm_num, timer_num, MCPWM_OPR_A, duty_cycle);
    mcpwm_set_duty_type(mcpwm_num, timer_num, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

static void set_pwm(void)
{
    mcpwm_gpio_initialize();
    printf("Configuring Initial Parameters of mcpwm...\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 1000; //frequency = 500Hz,
    pwm_config.cmpr_a = 0;       //duty cycle of PWMxA = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); //Configure PWM0A & PWM0B with above settings
}

static void set_configuration(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_0);

    gpio_reset_pin(LED_RED);
    gpio_reset_pin(LED_GREEN);
    gpio_reset_pin(LED_BLUE);
    gpio_reset_pin(LED);
    gpio_reset_pin(BUZZER);

    gpio_set_direction(LED_RED, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_GREEN, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED_BLUE, GPIO_MODE_OUTPUT);
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
    gpio_set_direction(BUZZER, GPIO_MODE_OUTPUT);
}

static int mappingValue(float x, float in_min, float in_max, int out_min, int out_max)
{
    int hasil = (int)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;

    return hasil;
}

static void SetAlarm(int pin, bool on)
{
    if (on)
    {
        for(int i = 0; i < 4; i++)
        {
            gpio_set_level(pin, 1);
            vTaskDelay(60 / portTICK_PERIOD_MS);
            gpio_set_level(pin, 0);
            vTaskDelay(60 / portTICK_PERIOD_MS);
        }
        vTaskDelay(520 / portTICK_PERIOD_MS);
    }
    else
    {
        gpio_set_level(pin, 0);
    }
}