#include "util.h"

void delay( volatile uint32_t cycles )
{
    volatile uint32_t i = 0;
    while (i < cycles) i++;
}
