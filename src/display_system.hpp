#pragma once

#include "i2c.hpp"

class Display
{
  public:
    Display( I2CMaster &i2c, unsigned char address );
    ~Display();

    void send_data(unsigned char byte);
    void send_command(unsigned char byte);

  private:
    const unsigned char I2C_BIT_RS = 0x1 << 0;
    const unsigned char I2C_BIT_RW = 0x1 << 1;
    const unsigned char I2C_BIT_E = 0x1 << 2;
    const unsigned char I2C_BIT_K = 0x1 << 3;
    const unsigned char I2C_BIT_DB4 = 0x1 << 4;
    const unsigned char I2C_BIT_DB5 = 0x1 << 5;
    const unsigned char I2C_BIT_DB6 = 0x1 << 6;
    const unsigned char I2C_BIT_DB7 = 0x1 << 7;
    
    const unsigned char INSTR_FUN_SET_4BIT = I2C_BIT_DB5;

    const unsigned char I2C_BIT_RS_DATA_INPUT = I2C_BIT_RS;
    const unsigned char I2C_BIT_RS_INSTR_INPUT = 0;
    const unsigned char I2C_BIT_RW_READ = I2C_BIT_RW;
    const unsigned char I2C_BIT_RW_WRITE = 0;

    I2CMaster &i2c;

    void set_to_4_bit_mode();
};
