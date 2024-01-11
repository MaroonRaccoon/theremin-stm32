#include "timer_test_system.hpp"

TimerTestSystem::TimerTestSystem( GPIO &button, GPIO &out, DelayTimer &delay )
    : button( button ), out( out ), delay( delay ), prev_button_down( false )
{
    out.unset();
}

void TimerTestSystem::tick()
{
    bool button_down = button.read();

    if ( prev_button_down && !button_down ) {
        out.set();
        delay.us(200);
        out.unset();

        delay.us(500);

        out.set();
        delay.us(150);
        out.unset();

        delay.us(500);

        out.set();
        delay.us(100);
        out.unset();
    }

    prev_button_down = button_down;
}
