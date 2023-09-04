#pragma once
#include "io.hpp"

class LED
{
  public:
    LED( GPIO &pin );
    void on();
    void off();
    void toggle();

    int read();

    void display_number( uint32_t num );

  private:
    GPIO pin;
};
