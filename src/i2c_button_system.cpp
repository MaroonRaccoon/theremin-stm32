#include "i2c_button_system.hpp"
#include "util.h"

I2CButtonSystem::I2CButtonSystem( I2CMaster &i2c, GPIO &button, LED &led )
    : i2c( i2c ), button( button ), led( led ), button_down( false )
{
}

void I2CButtonSystem::tick()
{
    bool prev_button_down = button_down;
    button_down           = button.read();

    if ( prev_button_down && !button_down ) {
        for ( int i = 0; i < 10; i++ ) {
            delay( 20000 );
            led.toggle();
        }
        delay( 50000 );

        unsigned char packet = 0;

        // disable RW bit
        packet &= ~0x2;

        i2c.start_write( 0x3f );
        i2c.write( 0x1 << 7UL );
        i2c.stop_write();
    }
}
