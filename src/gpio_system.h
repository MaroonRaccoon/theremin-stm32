#pragma once

#include "stm32f401xe.h"

typedef enum
{
    PUPDR_NONE = 0,
    PUPDR_UP,
    PUPDR_DOWN
} pupdr_config_t;

void gpio_setup_output(GPIO_TypeDef *port, int pin);
void gpio_setup_input(GPIO_TypeDef *port, int pin);
void gpio_set_pupdr(GPIO_TypeDef *port, int pin, pupdr_config_t pupdr_config);
void gpio_set_afr(GPIO_TypeDef *port, int pin, int afr_ix, int afr_val);
void gpio_set_mode_af(GPIO_TypeDef *port, int pin);

int gpio_read(GPIO_TypeDef *port, int pin);

void gpio_write(GPIO_TypeDef *port, int pin, int on);
void gpio_set(GPIO_TypeDef *port, int pin);
void gpio_unset(GPIO_TypeDef *port, int pin);
void gpio_toggle(GPIO_TypeDef *port, int pin);
