#pragma once

#include "ultrasonic.hpp"

struct InterruptHandlers
{
    Ultrasonic &tim1_up_tim10_handler;
};

extern InterruptHandlers *g_interrupt_handlers;
