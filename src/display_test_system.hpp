#pragma once

#include "display_system.hpp"
#include "io.hpp"
#include "timer.hpp"

class DisplayTestSystem
{
  public:
    DisplayTestSystem( Display &display, GPIO &button, DelayTimer &delay );

    void tick();

  private:
    Display    &display;
    GPIO       &button;
    DelayTimer &delay;
};
