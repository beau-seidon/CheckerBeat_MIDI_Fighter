#include <Arduino.h>

#include "activity_monitor.h"
#include "button_monitor.h"
#include "pot_monitor.h"
#include "joystick_monitor.h"
#include "midi_interface.h"



void setup()
{
    activity_setup();
    button_setup();
    pot_setup();
    joystick_setup();
    midi_setup();
}



void loop()
{
    button_handler();
    pot_handler();
    joystick_handler();
    activity_monitor(0);
}
