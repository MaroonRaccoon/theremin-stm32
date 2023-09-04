#include "util.h"

void delay( volatile uint32_t cycles )
{
    for ( volatile uint32_t i = 0; i < cycles; i++ )
        ;
}
