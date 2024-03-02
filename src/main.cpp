#include "audio.hpp"
#include "dac.hpp"
#include "io_constants.hpp"
#include "led_system.h"
#include "spi.hpp"
#include "ultrasonic_button_system.hpp"
#include "util.h"

#include "interrupts.hpp"

InterruptHandlers *g_interrupt_handlers = nullptr;

// ---- GPIO ----
// dac_spi setup for SPI2 peripheral
const auto dac_spi_nss_port    = gpio::Port::B;
const auto dac_spi_nss_pin     = 12;
const auto dac_spi_nss_afr_val = 5u;

const auto dac_spi_sck_port    = gpio::Port::B;
const auto dac_spi_sck_pin     = 13;
const auto dac_spi_sck_afr_val = 5u;

const auto dac_spi_mosi_port    = gpio::Port::B;
const auto dac_spi_mosi_pin     = 15;
const auto dac_spi_mosi_afr_val = 5u;

const auto ultrasonic_trigger_port = gpio::Port::B;
const auto ultrasonic_trigger_pin  = 9;

const auto ultrasonic_echo_port = gpio::Port::B;
const auto ultrasonic_echo_pin  = 8;

const auto isr_test_port = gpio::Port::C;
const auto isr_test_pin  = 0;

// ---- timers ----
const auto periph_ultrasonic_timer = timer::Id::Tim10;
const auto periph_delay_timer      = timer::Id::Tim11;

// TOOD: make a mutex using the synchronization instructions (LDREXW, LDREXH, etc) from programmer's manual

enum class State
{
    Waiting,
    FillingBuffer
};

int main( void )
{
    // clang-format off

    // delay timer
    DelayTimer timer_delay(periph_delay_timer);

    // ultrasonic setup
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
    Ultrasonic ultrasonic( pin_ultrasonic_trigger, pin_ultrasonic_echo, timer_ultrasonic_range, timer_delay );

    // DAC & audio driver setup
    GPIO pin_dac_nss(
        {
            .port       = dac_spi_nss_port,
            .pin        = dac_spi_nss_pin,
            .mode       = gpio::Mode::Output,
            .outputType = gpio::OutputType::OpenDrain,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::None
        },
        dac_spi_nss_afr_val
    );
    GPIO pin_dac_sck(
        {
            .port       = dac_spi_sck_port,
            .pin        = dac_spi_sck_pin,
            .mode       = gpio::Mode::AlternateFunction,
            .outputType = gpio::OutputType::PushPull,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::None
        },
        dac_spi_sck_afr_val
    );
    GPIO pin_dac_mosi(
        {
            .port       = dac_spi_mosi_port,
            .pin        = dac_spi_mosi_pin,
            .mode       = gpio::Mode::AlternateFunction,
            .outputType = gpio::OutputType::PushPull,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::None
        },
        dac_spi_mosi_afr_val
    );
    SPIMaster spi_dac(
        {
            .id = spi::ID::SPI_2,
            .not_chip_select = pin_dac_nss,
            .dataFrameFormat = spi::DataFrameFormat::Frame16Bit,
            .softwareSlaveManagement = false,
            .dataFrameOrder = spi::DataFrameOrder::MsbFirst,
            .baudRateControl = 7,
            .clockPolarity = spi::ClockPolarity::IdleLow,
            .clockPhase = spi::ClockPhase::SampleFirst,
        }
    );
    DAC dac( { .spi = spi_dac } );
    AudioDriver<128, 256> audio_driver(
        {
            .dac = dac,
            .sample_rate = 20000,
        }
    );

    // output for ISR timing test
    GPIO pin_isr_test(
        { 
            .port       = isr_test_port,
            .pin        = isr_test_pin,
            .mode       = gpio::Mode::Output,
            .outputType = gpio::OutputType::PushPull,
            .speed      = gpio::Speed::Low,
            .pull       = gpio::Pull::Down,
        }
    );

    // clang-format on

    // interrupt setup
    InterruptHandlers interrupt_handlers{ .tim1_up_tim10_handler = ultrasonic };
    g_interrupt_handlers = &interrupt_handlers;

    State state = State::Waiting;

    // testing timing for the ISR via logic analyzer
    while (1) {
        pin_isr_test.set();
        audio_driver.isr_write_dac();
        pin_isr_test.unset();
        timer_delay.ms(500);
    }

    /*
    // main loop
    uint16_t range = 0;
    while ( 1 ) {
        switch (state) {
            case State::Waiting:
                if (audio_driver.buffer_traversed) state = State::FillingBuffer;
                break;
            case State::FillingBuffer:
                range = ultrasonic.measure_range_mm();
                // limit to ~200mm measured
                if ( range > 200 )
                    range = 200;

                // scale to 0 -> 4000 (max is 4095)
                range *= 20;
                break;
        }


        dac.write( range );
    }
    */

    return 0;
}
