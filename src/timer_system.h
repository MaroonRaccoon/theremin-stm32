#pragma once

#include <stdint.h>

/*

TIM10/11 registers

- TIMx_CR1
  - CKD: clock division
  - ARPE: auto-reload preload enable
  - OPM: one-pulse mode
    - whether or not to stop counter (clear CEN) on next update event
  - UDIS: update disable
    - enable/disable update interrupt (UEV) event generation
  - CEN: counter enable

- TIMx_DIER: interrupt enable register
  - CC1IE: capture/compare 1 interrupt enable
  - UIE: update interrupt enable

- TIMx_SR: TIM status register
  - CC1OF: capture/compare 1 overcapture flag
    - set by hardware when: 
      - channel is configured in input capture mode
      - counter value has been captures in TIMx_CCR1 while CC1IF flag was already set
  - CC1IF: capture/compare 1 interrupt flag
    - if channel CC1 configured as output
      - set by hardware when counter matches compare value
      - cleared by software
    - if CC1 configured as input
      - set by hardware on a capture
        - edge detected on IC1 which matches selected polarity
      - cleared by hardware or by reading TIMx_CCR1
  - UIF: update interrupt flag
    - set by hardware on update event
    - cleared by software

- TIMx_EGR: event generation register
  - CC1G: capture/compare 1 generation
    - set by software, auto cleared by hardware
    - if channel CC1 configured as input
      - capture current counter value in TIMx_CCR1
      - set CC1IF
      - send interrupt if enabled
  - UG: update generation
    - set by software, auto cleared by hardware
    - re-init counter, update registers, clear prescaler counter (but leave prescaler ratio the same)
- TIMx_CCMR1: capture/compare mode register 1
  - CC1S: capture/compare 1 selection
    - direction of channel (input/output), as well as the used input
    - 00: CC1 channel is configured as output
    - 01: CC1 channel is configured as input, IC1 is mapped on TI1
  - input capture mode
    - IC1F: input capture 1 filter
      - frequency used to sample TI1 input
      - length of digital filter applied to TI1 (number of consecutive events needed to validate transition on output)
    - IC1PSC: input capture 1 prescaler
      - ratio of prescaler acting on CC1 input (IC1)
      - how many events needed to do capture
- TIMx_CCER: capture/compare enable register
  - CC1NP: capture/compare 1 complementary output polarity
    - if CC1 configured as input: used with CC1P to define TI1FP1 polarity
  - CC1P: capture/compare 1 output polarity
    - if CC1 configured as input:
      - "inverted" refers to TI1FP1 inverted
      - 00: noninverted, capture on rising edge
      - 01: inverted, falling edge
      - 11: noninverted both edges
  - CC1E: capture/compare 1 output enable
    - if CC1 configured as input: determines if counter value can be captured into TIMx_CCR1
- TIMx_CNT: counter
- TIMx_PSC: prescaler
- TIMx_ARR: auto-reload register


- TIMx_CCR1
  - if channel CC1 is configured as input...
      - val is counter value transferred by last input capture 1 event (IC1)
      - read-only

- TIM11_OR: option register

TIMx_CH1 -(TI1)-> [Input filter & edge detector] -(IC1)-> [Prescaler] -(IC1PS)->  [Capture/Compare 1 register]

*/

void timer_setup();
void timer_delay(uint32_t ms);


