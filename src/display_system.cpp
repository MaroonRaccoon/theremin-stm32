#include "display_system.h"

#include "i2c_system.h"
#include "gpio_system.h"
#include "io_constants.h"

void display_setup_gpio()
{
    // enable clock
    DISPLAY_I2C_RCC |= DISPLAY_I2C_RCC_EN;

    // scl gpio
    gpio_setup_output(DISPLAY_I2C_SCL_PORT, DISPLAY_I2C_SCL_PIN);
    gpio_set_pupdr(DISPLAY_I2C_SCL_PORT, DISPLAY_I2C_SCL_PIN, PUPDR_UP);
    gpio_set_mode_af(DISPLAY_I2C_SCL_PORT, DISPLAY_I2C_SCL_PIN);

    // sda gpio
    gpio_setup_output(DISPLAY_I2C_SDA_PORT, DISPLAY_I2C_SDA_PIN);
    gpio_set_pupdr(DISPLAY_I2C_SDA_PORT, DISPLAY_I2C_SDA_PIN, PUPDR_UP);
    gpio_set_mode_af(DISPLAY_I2C_SDA_PORT, DISPLAY_I2C_SDA_PIN);

    // scl af
    uint32_t display_i2c_scl_afr_shift = DISPLAY_I2C_SCL_PIN * 4U;
    DISPLAY_I2C_SCL_PORT->AFR[DISPLAY_I2C_SCL_AFR_IX] &= ~(0xFU << display_i2c_scl_afr_shift);
    DISPLAY_I2C_SCL_PORT->AFR[DISPLAY_I2C_SCL_AFR_IX] |= ~(DISPLAY_I2C_SCL_AFR_VAL << display_i2c_scl_afr_shift);

    // sda af
    uint32_t display_i2c_sda_afr_shift = DISPLAY_I2C_SDA_PIN * 4U;
    DISPLAY_I2C_SDA_PORT->AFR[DISPLAY_I2C_SDA_AFR_IX] &= ~(0xFU << display_i2c_sda_afr_shift);
    DISPLAY_I2C_SDA_PORT->AFR[DISPLAY_I2C_SDA_AFR_IX] |= ~(DISPLAY_I2C_SDA_AFR_VAL << display_i2c_sda_afr_shift);
}

void display_setup()
{
    display_setup_gpio();
    i2c_setup(DISPLAY_I2C);
}

void display_start()
{
    i2c_start_tx(DISPLAY_I2C, 0x37);
}
