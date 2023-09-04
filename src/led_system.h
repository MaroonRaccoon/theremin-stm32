#pragma once

#include <stdint.h>
typedef struct {
    int counter;
    int cutoff;
} LedState;

void led_setup();
void led_toggle();
void led_on();
void led_off();
int led_button_read();
void led_tick();
void led_display_number(uint32_t num);
