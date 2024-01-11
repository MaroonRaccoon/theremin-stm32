#pragma once

#include "io.hpp"
#include "led.hpp"
#include "timer.hpp"

class Ultrasonic
{
  public:
    Ultrasonic( GPIO &trigger, GPIO &echo, BasicInputCaptureTimer &timer, DelayTimer &delay );

    uint32_t measure_range_mm();

    void isr_timer();

    enum class Status
    {
        Idle,
        WaitingForEcho,
        ReadingEcho,
        Done
    };

  private:
    GPIO                   &trigger;
    GPIO                   &echo;
    BasicInputCaptureTimer &timer;
    DelayTimer             &delay;
    volatile int num_interrupts;

    volatile Status   status;
    volatile uint32_t distance_mm;
};
