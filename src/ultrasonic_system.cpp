#include "ultrasonic_system.h"
#include "gpio_system.h"
#include "io_constants.h"
#include "led_system.h"
#include "util.h"

#include "stm32f401xe.h"

volatile static ultrasonic_status_t state = ULT_IDLE;
volatile static uint32_t distance_mm = 0;
volatile static int new_distance_ready = 0;

// pulse from echo pin will be between 58 us and 2ms
// board is 16MHz by default, giving us 16 cycles/us

void ultrasonic_tick(ultrasonic_state_t *state)
{
    int prev_button_down = state->button_down;
    state->button_down = ultrasonic_button_read();

    if (prev_button_down && !state->button_down) {
        ultrasonic_trigger();
    }
}

int ultrasonic_button_read()
{
    return gpio_read(ULT_BUTTON_PORT, ULT_BUTTON_PIN);
}

void ultrasonic_timer_setup()
{
    // enable clock
    ULT_TIM_RCC |= ULT_TIM_RCC_EN_MSK;

    // configure CC1 as input
    ULT_TIM->CCMR1 &= ~0x7fU; // reset
    ULT_TIM->CCMR1 |= 0x1U; // configure as input

    // enable capture/compare interrupt
    ULT_TIM->DIER &= ~0x3U; // reset
    ULT_TIM->DIER |= TIM_DIER_CC1IE; // enable capture/compare 1 interrupt

    // sensitive to rising and falling edges, enable capture
    ULT_TIM->CCER &= ~((0x1U << 3) | 0x3U); // reset;
    ULT_TIM->CCER = (0x1U << 3) | (0x1U << 1) | 0x1;

    NVIC_ClearPendingIRQ(TIM1_UP_TIM10_IRQn);
    NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 4U);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

    //TIM10->CR1 |= 0x1;
}

void ultrasonic_gpio_setup()
{
    // enable peripheral clocks
    ULT_ECHO_RCC |= ULT_ECHO_RCC_EN_MSK;
    ULT_TRIGGER_RCC |= ULT_TRIGGER_RCC_EN_MSK;
    ULT_BUTTON_RCC |= ULT_BUTTON_RCC_EN_MSK;

    // setup trigger output
    gpio_setup_output(ULT_TRIGGER_PORT, ULT_TRIGGER_PIN);
    gpio_set_pupdr(ULT_TRIGGER_PORT, ULT_TRIGGER_PIN, PUPDR_DOWN);

    // setup button input
    gpio_setup_input(ULT_BUTTON_PORT, ULT_BUTTON_PIN);
    gpio_set_pupdr(ULT_BUTTON_PORT, ULT_BUTTON_PIN, PUPDR_DOWN);

    // setup echo input
    ULT_ECHO_PORT->MODER &= ~(0x3U << (ULT_ECHO_PIN * 2)); // reset pin mode
    ULT_ECHO_PORT->MODER |= (0x2U << (ULT_ECHO_PIN * 2)); // set pin to alternate function mode

    ULT_ECHO_PORT->PUPDR &= ~(0x3U << (ULT_ECHO_PIN * 2)); // reset pin pull-up/pull-down
    ULT_ECHO_PORT->PUPDR |= (0x2U << (ULT_ECHO_PIN * 2)); // set pin to pull-down

    int ult_afr_shift = (ULT_ECHO_PIN - 8 * ULT_ECHO_AFR_IX) * 4;
    ULT_ECHO_PORT->AFR[ULT_ECHO_AFR_IX] &= ~(0xFU << ult_afr_shift);
    ULT_ECHO_PORT->AFR[ULT_ECHO_AFR_IX] |= ULT_ECHO_AFR_VAL << ult_afr_shift; // set alternate function
}


void ultrasonic_clear_interrupt_flag()
{
    ULT_TIM->SR &= ~0x2U;
}

// peripheral registers have to be written by half-words (16 bits) or words (32 bits)
// read accesses can be done by bytes, half-words, or words
void ultrasonic_setup()
{
    ultrasonic_gpio_setup();
    ultrasonic_timer_setup();
}

int ultrasonic_get_distance_mm(uint32_t *out_distance)
{
    if (new_distance_ready) {
        new_distance_ready = 0;
        *out_distance = distance_mm;
        return 1;
    } else return 0;
}

void ultrasonic_trigger() {
    switch(state) {
        case ULT_IDLE:
            gpio_write(ULT_TRIGGER_PORT, ULT_TRIGGER_PIN, 1U);
            state = ULT_WAITING_FOR_ECHO;
            delay(5000);
            gpio_write(ULT_TRIGGER_PORT, ULT_TRIGGER_PIN, 0U);
            break;
        default:
            return;
    }
}

extern "C" 
{
void isr_ultrasonic()
{
    uint32_t counter_val;
    switch(state) {
        case ULT_WAITING_FOR_ECHO:
            gpio_write(ULT_TRIGGER_PORT, ULT_TRIGGER_PIN, 0U);
            ULT_TIM->CNT &= ~0xFFFFU;
            ULT_TIM->CR1 |= 1U;
            state = ULT_READING_ECHO;
            break;
        case ULT_READING_ECHO:
            counter_val = ULT_TIM->CCR1;
            distance_mm = 343 * counter_val / 16 / 2 / 1000;
            new_distance_ready = 1;
            ULT_TIM->CR1 &= ~1U;
            state = ULT_IDLE;
            break;
        default:
            break;
    }
    ultrasonic_clear_interrupt_flag();
}

} // extern "C"
