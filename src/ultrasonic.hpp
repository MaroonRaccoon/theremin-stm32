#pragma once

#include "io.hpp"
#include "led.hpp"
#include "timer.hpp"

class Ultrasonic
{
  public:
    Ultrasonic( GPIO &trigger, GPIO &echo, BasicInputCaptureTimer &timer );

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

    volatile Status   status;
    volatile uint32_t distance_mm;
};
