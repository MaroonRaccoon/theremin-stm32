#pragma once

#include "led.hpp"
#include "io.hpp"
#include "ultrasonic.hpp"

class UltrasonicButtonSystem
{
  public:
    UltrasonicButtonSystem( Ultrasonic &ultrasonic, GPIO &trigger_button, LED &led );
    void tick();

  private:
    Ultrasonic &ultrasonic;
    GPIO &trigger_button;
    LED &led;

    bool button_down;
};
