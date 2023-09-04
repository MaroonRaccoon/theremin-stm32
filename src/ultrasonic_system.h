#pragma once

#include <stdint.h>

typedef enum {
    ULT_IDLE,
    ULT_WAITING_FOR_ECHO,
    ULT_READING_ECHO
} ultrasonic_status_t;

typedef struct {
    int button_down;
} ultrasonic_state_t;

void ultrasonic_setup();
void ultrasonic_trigger();
int ultrasonic_get_distance_mm(uint32_t *out_distance);
void ultrasonic_tick(ultrasonic_state_t *state);

int ultrasonic_button_read();
int ultrasonic_button_pressed();
