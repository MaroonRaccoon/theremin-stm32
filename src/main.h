#pragma once

#include <stdint.h>
#include "stm32f401xe.h"

void setup_gpio_output(GPIO_TypeDef *gpio, int pin);
void setup_gpio_input(GPIO_TypeDef *gpio, int pin);

void handle_input(int *curr, int *prev, GPIO_TypeDef *gpio, int pin);
