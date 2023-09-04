#include "led.hpp"
#include "util.h"

LED::LED( GPIO &pin ) : pin( pin ) { }

void LED::toggle()
{
    pin.toggle();
}

void LED::on()
{
    pin.set();
}

void LED::off()
{
    pin.unset();
}

int LED::read()
{
    return pin.read();
}

void LED::display_number( uint32_t num )
{
    uint32_t indicator_time         = 100000U;
    uint32_t mid_digit_on_time      = 30000U;
    uint32_t mid_digit_between_time = mid_digit_on_time / 2;
    uint32_t between_digit_time     = mid_digit_on_time * 2;

    off();

    on();
    delay( indicator_time );
    off();

    int digit;
    do {
        digit = num % 10;
        for ( int i = 0; i < digit + 1; i++ ) {
            on();
            delay( mid_digit_on_time );
            off();
            delay( mid_digit_between_time );
        }
        num /= 10;
        delay( between_digit_time );
    } while ( num > 0 );
}
