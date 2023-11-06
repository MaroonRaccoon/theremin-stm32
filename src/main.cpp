#include "display_system.hpp"
#include "display_test_system.hpp"
#include "i2c.hpp"
#include "i2c_button_system.hpp"
#include "io_constants.hpp"
#include "led.hpp"
#include "timer.hpp"
#include "ultrasonic.hpp"
#include "ultrasonic_button_system.hpp"
#include "util.h"
#include "timer_test_system.hpp"

#include "interrupts.hpp"

InterruptHandlers *g_interrupt_handlers = nullptr;

const auto ultrasonic_led_port = gpio::Port::A;
const auto ultrasonic_led_pin  = 5;

const auto ultrasonic_trigger_button_port = gpio::Port::A;
const auto ultrasonic_trigger_button_pin  = 6;

const auto ultrasonic_trigger_port = gpio::Port::B;
const auto ultrasonic_trigger_pin  = 9;

const auto ultrasonic_echo_port = gpio::Port::B;
const auto ultrasonic_echo_pin  = 8;

const auto periph_ultrasonic_timer = timer::Id::Tim10;
const auto periph_delay_timer      = timer::Id::Tim10;

const auto periph_display_i2c  = i2c::ID::I2C_1;
const auto display_i2c_port    = gpio::Port::B;
const auto display_i2c_scl_pin = 6;
const auto display_i2c_sda_pin = 7;

const auto timer_test_port = gpio::Port::C;
const auto timer_test_pin = 8;

// TOOD: make a mutex using the synchronization instructions (LDREXW, LDREXH, etc) from programmer's manual
// TODO: check privilege stuff for interrupts

int main( void )
{
    // clang-format off
    GPIO pin_ultrasonic_trigger(
        { 
            .port       = ultrasonic_trigger_port,
            .pin        = ultrasonic_trigger_pin,
            .mode       = gpio::Mode::Output,
            .outputType = gpio::OutputType::PushPull,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Down,
        }
    );
    GPIO pin_ultrasonic_led(
        { 
            .port       = ultrasonic_led_port,
            .pin        = ultrasonic_led_pin,
            .mode       = gpio::Mode::Output,
            .outputType = gpio::OutputType::PushPull,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Down,
        }
    );
    GPIO button_ultrasonic_trigger(
        { 
            .port       = ultrasonic_trigger_button_port,
            .pin        = ultrasonic_trigger_button_pin,
            .mode       = gpio::Mode::Input,
            .outputType = gpio::OutputType::OpenDrain,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Down,
        }
    );
    GPIO pin_ultrasonic_echo(
        { 
            .port       = ultrasonic_echo_port,
            .pin        = ultrasonic_echo_pin,
            .mode       = gpio::Mode::AlternateFunction,
            .outputType = gpio::OutputType::OpenDrain,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Down,
        },
        ULT_ECHO_AFR_VAL
    );
    GPIO pin_display_i2c_scl(
        { 
            .port       = display_i2c_port,
            .pin        = display_i2c_scl_pin,
            .mode       = gpio::Mode::AlternateFunction,
            .outputType = gpio::OutputType::OpenDrain,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Up,
        },
        AFR_VAL_I2C
    );
    GPIO pin_display_i2c_sda(
        { 
            .port       = display_i2c_port,
            .pin        = display_i2c_sda_pin,
            .mode       = gpio::Mode::AlternateFunction,
            .outputType = gpio::OutputType::OpenDrain,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Up,
        },
        AFR_VAL_I2C
    );
    GPIO pin_timer_test_out(
        {
            .port = timer_test_port,
            .pin = timer_test_pin,
            .mode = gpio::Mode::Output,
            .outputType = gpio::OutputType::PushPull,
            .speed = gpio::Speed::Low,
            .pull = gpio::Pull::None,
        }
    );

    BasicInputCaptureTimer timer_ultrasonic_range(
        periph_ultrasonic_timer,
        { 
            .clockDivision   = timer::ClockDivision::InternalClock,
            .interruptEnable = true,
            .filter          = timer::InputCaptureFilter::None,
            .prescaler       = timer::InputCapturePrescaler::Every,
            .polarity        = timer::CaptureCompareInputPolarity::BothEdges 
        }
    );
    DelayTimer timer_delay(periph_delay_timer);

    LED led( pin_ultrasonic_led );
    Ultrasonic ultrasonic( pin_ultrasonic_trigger, pin_ultrasonic_echo, timer_ultrasonic_range, timer_delay );
    UltrasonicButtonSystem ultrasonic_button_system( ultrasonic, button_ultrasonic_trigger, led );

    I2CMaster display_i2c(
        {
            .id = periph_display_i2c,
            .peripheral_clock_frequency_MHz = 16U,
            .scl_clock_time_coeff = 76,
            .max_rise_time_ns = 1000,
            .master_mode = i2c::MasterMode::Slow,
        }
    );
    Display display(
        display_i2c,
        0x3f
    );
    // clang-format on

    DisplayTestSystem display_test(display, button_ultrasonic_trigger, timer_delay);

    InterruptHandlers interrupt_handlers{ .tim1_up_tim10_handler = ultrasonic };
    g_interrupt_handlers = &interrupt_handlers;

    while ( 1 ) {
        display_test.tick();
    }

    return 0;
}
