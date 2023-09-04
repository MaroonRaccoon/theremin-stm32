#include "ultrasonic_button_system.hpp"
#include "led.hpp"
#include "util.h"

UltrasonicButtonSystem::UltrasonicButtonSystem( Ultrasonic &ultrasonic, GPIO &trigger_button, LED &led )
    : ultrasonic( ultrasonic ), trigger_button( trigger_button ), led(led), button_down(false)
{
}

void UltrasonicButtonSystem::tick()
{
    bool prev_button_down = button_down;
    button_down = trigger_button.read();

    if (prev_button_down && !button_down) {
        uint32_t distance_mm = ultrasonic.measure_range_mm();
        if (distance_mm) led.display_number(distance_mm);
    }
}
