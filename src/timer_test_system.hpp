#pragma once

#include "io.hpp"
#include "timer.hpp"

class TimerTestSystem
{
  public:
    TimerTestSystem(GPIO &button, GPIO &out, DelayTimer &delay);
    void tick();

  private:
    GPIO &button;
    GPIO &out;
    DelayTimer &delay;

    bool prev_button_down;
};
