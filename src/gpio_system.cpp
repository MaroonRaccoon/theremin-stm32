#include "gpio_system.h"

void gpio_setup_output(GPIO_TypeDef *port, int pin)
{
    port->MODER &= ~(0x3 << (pin * 2)); // reset state

    port->OTYPER &= ~(0x1 << pin); // output push-pull
    port->OSPEEDR &= ~(0x3 << (pin * 2)); // low speed (??)
    port->PUPDR &= ~(0x3 << (pin * 2)); // no pull-up or pull-down
    port->ODR &= ~(0x1 << pin); // output

    port->MODER |= 0x1 << (pin * 2); // 01: General purpose output mode
}

void gpio_setup_input(GPIO_TypeDef *port, int pin)
{
    port->MODER &= ~(0x3 << (pin * 2)); // reset state/input mode

    port->OTYPER &= ~(0x1 << pin); // reset state
    port->OSPEEDR &= ~(0x3 << (pin * 2)); // low speed (??)
    port->PUPDR &= ~(0x3 << (pin * 2)); // no pull-up or pull-down
}

void gpio_set_pupdr(GPIO_TypeDef *port, int pin, pupdr_config_t pupdr_config)
{
    switch(pupdr_config) {
        case PUPDR_NONE:
            port->PUPDR &= ~(0x3 << (pin * 2));
            break;
        case PUPDR_UP:
            //port->PUPDR &= ~(0x2 << (pin * 2));
            port->PUPDR &= ~(0x3 << (pin * 2));
            port->PUPDR |= (0x1 << (pin * 2));
            break;
        case PUPDR_DOWN:
            //port->PUPDR &= ~(0x1 << (pin * 2));
            port->PUPDR &= ~(0x3 << (pin * 2));
            port->PUPDR |= (0x2 << (pin * 2));
            break;
    }
}

void gpio_set_mode_af(GPIO_TypeDef *port, int pin)
{
    port->MODER &= ~(0x3 << (pin * 2)); // reset state/input mode
    port->MODER |= (0x2 << (pin * 2)); // reset state/input mode
}

void gpio_set_afr(GPIO_TypeDef *port, int pin, int afr_ix, int afr_val)
{
    port->AFR[afr_ix] = afr_val;
}

int gpio_read(GPIO_TypeDef *port, int pin)
{
    return port->IDR & (0x1 << pin);
}

void gpio_write(GPIO_TypeDef *port, int pin, int on)
{
    if (on) gpio_set(port, pin);
    else gpio_unset(port, pin);
}

void gpio_set(GPIO_TypeDef *port, int pin)
{
    port->ODR |= (0x1 << pin);
}

void gpio_unset(GPIO_TypeDef *port, int pin)
{
    port->ODR &= ~(0x1 << pin);
}

void gpio_toggle(GPIO_TypeDef *port, int pin)
{
    port->ODR ^= (0x1 << pin);
}
