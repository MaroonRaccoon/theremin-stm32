#include "interrupts.hpp"

#include "io_constants.h"
#include "ultrasonic.hpp"
#include "util.h"

extern "C"
{
    // handles ultrasonic echo line edges to measure pulse length
    void isr_tim1_up_tim10( void )
    {
        if (!g_interrupt_handlers) return;
        g_interrupt_handlers->tim1_up_tim10_handler.isr_timer();
    }
}
