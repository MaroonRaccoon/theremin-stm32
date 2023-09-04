#include "led_system.h"
#include "gpio_system.h"
#include "io_constants.h"
#include "util.h"

void led_setup()
{
    LED_RCC |= LED_RCC_EN_MSK;
    LED_BUTTON_RCC |= LED_BUTTON_RCC_EN_MSK;
    gpio_setup_output(LED_PORT, LED_PIN);
    gpio_setup_input(LED_BUTTON_PORT, LED_BUTTON_PIN);
    gpio_set_pupdr(LED_BUTTON_PORT, LED_BUTTON_PIN, PUPDR_DOWN);
}

void led_toggle()
{
    gpio_toggle(LED_PORT, LED_PIN);
}

void led_on()
{
    gpio_write(LED_PORT, LED_PIN, 1U);
}

void led_off()
{
    gpio_write(LED_PORT, LED_PIN, 0U);
}

int led_button_read()
{
    return gpio_read(LED_BUTTON_PORT, LED_BUTTON_PIN);
}

void led_tick(LedState *state)
{
    state->counter++;
    if (state->counter % state->cutoff == 0 && led_button_read() == 0) {
        led_toggle();
        state->counter = 0;
    }
}

void led_display_number(uint32_t num)
{
    uint32_t indicator_time = 1000000U;
    uint32_t mid_digit_on_time = 300000U;
    uint32_t mid_digit_between_time = mid_digit_on_time / 2;
    uint32_t between_digit_time = mid_digit_on_time * 2;

    led_off();

    led_on();
    delay(indicator_time);
    led_off();

    int digit;
    do {
        digit = num % 10;
        for (int i = 0; i < digit + 1; i++) {
            led_on();
            delay(mid_digit_on_time);
            led_off();
            delay(mid_digit_between_time);
        }
        num /= 10;
        delay(between_digit_time);
    } while (num > 0);
}

