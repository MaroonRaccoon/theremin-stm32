#pragma once

#include "stm32f401xe.h"

/*

- setting START bit causes interface to generate start condition and switch to master mode (pg 481)
- master mode
  - MSL bit set

- set START bit
- send address byte, with LSB reset for transmitter mode
- hardware sets ADDR bit, generates interrupt if ITEVFEN is set
- master sends bytes from DR register to SDA line
  - master waits until first data byte is written to I2C_DR
- when acknowledge pulse is received
  - TxE bit set by hardware
  - interrupt is generated if ITEVFEN an ITBUFEN bits are set
- if TxE is set and data byte was not written to DR before end of last data transmission
  - BTF set
  - interface waits until BTF cleared by write to I2C_DR, stretching SCL low
- set STOP bit
  - should be programmed during EV8_2 event, when either TxE or BTF is set
- interface goes back to slave mode

if clock stretching enabled (p486)
- transmitter mode, if TxE=1 and BTF=1
  - interface holds clock line low before transmission to wait for microcontroller to write the byte into data register

registers
I2C_CR1: Control register 1
I2C_CR2: Control register 2
I2C_OAR1: Own address register 1
I2C_OAR2: Own address register 2
I2C_DR: Data register
I2C_SR1: Status register 1
I2C_SR2: Status register 2
I2C_CCR: Clock control register
I2C_TRISE: TRISE register
I2C_FLTR: FLTR register

*/

void i2c_setup(I2C_TypeDef *i2c);

void i2c_start_tx(I2C_TypeDef *i2c, char address);
void i2c_write(I2C_TypeDef *i2c, char data);
void i2c_stop(I2C_TypeDef *i2c);
