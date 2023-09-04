#include "i2c_system.h"
#include "stm32f401xe.h"

void i2c_setup(I2C_TypeDef *i2c)
{
    i2c->CR2 &= ~I2C_CR2_FREQ_Msk;
    i2c->CR2 |= I2C_CR2_FREQ_4; // 16 MHz

    // Sm mode (slow)
    i2c->CCR &= ~I2C_CCR_FS;

    // rise time
    i2c->TRISE &= ~I2C_TRISE_TRISE;
    i2c->TRISE |= 1000UL; // 1 us

    // clock control register (high/low time)
    // 4.7us / (1 / (16 MHz)) = 75.2 -> round up to 128 = 2^7
    i2c->CCR &= ~I2C_CCR_CCR;
    i2c->CCR |= 1UL << (7 - 1);

    // peripheral enable
    i2c->CR1 |= I2C_CR1_PE;
}

void i2c_start_tx(I2C_TypeDef *i2c, char address)
{
    i2c->CR1 |= I2C_CR1_START;
    i2c_write(i2c, address << 1);
    while (i2c->SR1 & I2C_SR1_ADDR);
}

void i2c_stop(I2C_TypeDef *i2c)
{
    // TODO: clear this sometime
    i2c->CR1 |= I2C_CR1_STOP;
}

void i2c_write(I2C_TypeDef *i2c, char data)
{
    *((char *) &i2c->DR) = data;
}

// TODO:
// make display system
// - do writes
