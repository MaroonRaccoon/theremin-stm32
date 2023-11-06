#include "display_system.hpp"

Display::Display( I2CMaster &i2c, unsigned char address ) : i2c( i2c )
{ 
    i2c.start_write( address );
    // TODO: delay?
    set_to_4_bit_mode();
}

Display::~Display()
{
    i2c.stop_write();
}

void Display::set_to_4_bit_mode()
{
    i2c.write(INSTR_FUN_SET_4BIT);
}
