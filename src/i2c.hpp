#pragma once

#include "stm32f401xe.h"

namespace i2c
{

enum class MasterMode
{
    Slow,
    Fast
};

enum class ID
{
    // underscores because I2C1, I2C2, I2C3 are macros
    I2C_1,
    I2C_2,
    I2C_3,
};

struct Settings
{
    ID id;
    uint32_t     peripheral_clock_frequency_MHz;
    uint32_t     scl_clock_time_coeff;
    uint32_t     max_rise_time_ns;
    MasterMode   master_mode;
};

} // namespace i2c

class I2CMaster
{
  public:
    I2CMaster( i2c::Settings settings );

    void start_write( unsigned char address );
    void stop_write();
    void write(unsigned char data);

  private:
    I2C_TypeDef *i2c;
    bool is_start_condition_generated();
    bool is_bus_busy();
    void wait_until_address_transmitted();
};
