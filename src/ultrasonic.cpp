#include "ultrasonic.hpp"
#include "util.h"

Ultrasonic::Ultrasonic( GPIO &trigger, GPIO &echo, BasicInputCaptureTimer &timer )
    : trigger( trigger ), echo( echo ), timer( timer ), status( Status::Idle )
{
    trigger.unset();
}

uint32_t Ultrasonic::measure_range_mm()
{
    if ( status != Status::Idle )
        return 0;

    status = Status::WaitingForEcho;
    trigger.set();
    delay( 100000 );
    trigger.unset();

    // while we're waiting here, the ultrasonic sensor should send a pulse through the echo line
    // on the downward edge of this pulse, the status will be set to Done by the interrupt, and the
    // distance will be set
    while ( status != Status::Done )
        ;

    status = Status::Idle;
    return distance_mm;
}

void Ultrasonic::isr_timer()
{
    switch ( status ) {
    case Status::WaitingForEcho: {
        timer.restart();
        status = Status::ReadingEcho;
        break;
    }
    case Status::ReadingEcho: {
        timer.stop();
        uint32_t us = timer.read_us();
        // speed of sound: 0.343 mm / us
        distance_mm = ( 343 * us / 1000 ) / 2;
        status      = Status::Done;
        break;
    }
    default:
        break;
    }
    timer.clear_interrupt_flag();
}
