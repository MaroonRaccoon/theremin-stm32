#include "display_test_system.hpp"

DisplayTestSystem::DisplayTestSystem( Display &display, GPIO &button, DelayTimer &delay )
    : display( display ), button( button ), delay( delay )
{
}

void DisplayTestSystem::tick()
{
    static bool prev_button_down = false;
    bool button_down = button.read();

    if (prev_button_down && !button_down) {
        // do something
    }

    prev_button_down = button_down;
}
