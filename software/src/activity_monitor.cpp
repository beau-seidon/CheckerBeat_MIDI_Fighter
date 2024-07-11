#include "activity_monitor.h"



unsigned long T_led = millis();  // activity LED timer



void activity_setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}



void activity_monitor(int mode)
{
    if (mode) {
        digitalWrite(LED_BUILTIN, HIGH);
        T_led = millis();
    } else {
        if((millis() - T_led) > 50) digitalWrite(LED_BUILTIN, LOW);
    }
}
