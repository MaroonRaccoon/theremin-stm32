#pragma once

#include "i2c.hpp"
#include "io.hpp"
#include "led.hpp"

class I2CButtonSystem
{
  public:
    I2CButtonSystem( I2CMaster &i2c, GPIO &button, LED &led );
    void tick();

  private:
    I2CMaster &i2c;
    GPIO      &button;
    LED       &led;

    bool button_down;
};
