#include "i2c.hpp"
#include "util.h"

I2CMaster::I2CMaster( i2c::Settings settings )
{
    switch(settings.id) {
        case i2c::ID::I2C_1:
            i2c = I2C1;
            RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
            break;
        case i2c::ID::I2C_2:
            i2c = I2C2;
            RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
            break;
        case i2c::ID::I2C_3:
            i2c = I2C3;
            RCC->APB1ENR |= RCC_APB1ENR_I2C3EN;
            break;
    }

    i2c->CR2 &= ~I2C_CR2_FREQ;
    i2c->CR2 |= settings.peripheral_clock_frequency_MHz;

    i2c->CCR &= ~( I2C_CCR_CCR | I2C_CCR_DUTY | I2C_CCR_FS );
    i2c->CCR |= settings.scl_clock_time_coeff;
    if ( settings.master_mode == i2c::MasterMode::Fast )
        i2c->CCR |= I2C_CCR_FS;

    /*
    max_rise_time / clk_period = max_rise_time / (1 / periph_clock_freq)
    = max_rise_time_ns * 10^(-9) * periph_clock_freq_MHz * 10^6)
    = max_rise_time_ns * periph_clock_freq_MHz / 1000
    increment by one per the datasheet
    */
    i2c->TRISE &= ~I2C_TRISE_TRISE;
    i2c->TRISE |= settings.max_rise_time_ns * settings.peripheral_clock_frequency_MHz / 1000 + 1;

    i2c->CR1 |= I2C_CR1_PE;
}

void I2CMaster::write( unsigned char data )
{
    *reinterpret_cast< volatile unsigned char * >( &( i2c->DR ) ) = data;
}

void I2CMaster::start_write(unsigned char address)
{
    i2c->CR1 |= I2C_CR1_START;
    while (!is_start_condition_generated());
    i2c->CR1 &= ~I2C_CR1_START;

    // transmit 7-bit address
    write(address << 1); // LSB=0 for transmit mode

    wait_until_address_transmitted();
}

void I2CMaster::stop_write()
{
    // TODO: clear this sometime
    i2c->CR1 |= I2C_CR1_STOP;
}

bool I2CMaster::is_start_condition_generated()
{
    return i2c->SR1 & I2C_SR1_SB;
}

bool I2CMaster::is_bus_busy()
{
    return i2c->SR2 & I2C_SR2_BUSY;
}

void I2CMaster::wait_until_address_transmitted()
{
    // wait until the address was transmitted
    while (!(i2c->SR1 & I2C_SR1_ADDR));
    // TODO: if the compiler optimizes this out then transmission won't work
    int _ = i2c->SR2;
}
