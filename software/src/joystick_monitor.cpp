#include "joystick_monitor.h"



const int JOYSTICK_PINS[MAX_AXES] = { 35, 34 };
const int JOYSTICK_DEADBAND = (4096 / 128) - 1;
const int AXIS_CHANGE_DELAY = 5;  // ms

const int AXIS_CENTER[MAX_AXES] = { 1895, 1793 };

int active_axis = 0;

int axis_values[MAX_AXES] = { 0 };
int prev_axis_values[MAX_AXES] = { 0 };

static unsigned long T_act = millis();        // active-axis change timer
unsigned long T_js[MAX_AXES] = { millis() };  // joystick debounce timers

// private declarations:
void read_axis(int axis);
void axis_to_midi(int axis);
int joystick_map(int x, int in_min, int in_max, int out_min, int out_max, int center);



void joystick_setup()
{
    pinMode(JOYSTICK_PINS[0], INPUT);  // X-Axis (mod wheel)
    pinMode(JOYSTICK_PINS[1], INPUT);  // Y-Axis (pitch wheel)

    for (int j = 0; j < MAX_AXES; ++j) { prev_axis_values[j] = axis_values[j]; }
}



void joystick_handler()
{
    unsigned long T = millis();
    if (T > T_act + AXIS_CHANGE_DELAY) {
        T_act = T;
        ++active_axis;
        if (active_axis >= MAX_AXES) active_axis = 0;
    }

    read_axis(active_axis);
    axis_to_midi(active_axis);
}



void read_axis(int axis)
{
    int axis_raw_value = analogRead(JOYSTICK_PINS[axis]);  // charge the ADC
    axis_raw_value = analogRead(JOYSTICK_PINS[axis]);

    int centered_value = joystick_map(axis_raw_value, 0, 4095, 0, 4095, AXIS_CENTER[axis]);

    if (abs(axis_values[axis] - centered_value) > JOYSTICK_DEADBAND) {  // prevent ADC jitter and noise
        axis_values[axis] = centered_value;
        // Serial.print(axis); Serial.print(" "); Serial.println(axis_values[axis]);
    }
}



void axis_to_midi(int axis)
{
    if(axis_values[axis] != prev_axis_values[axis]) {
        prev_axis_values[axis] = axis_values[axis];

        midi_sendCC_js(axis, axis_values[axis]);
    }
}



int joystick_map(int x, int in_min, int in_max, int out_min, int out_max, int center)
{
    if (x > center) {
        in_min = center;
        out_min = out_max / 2;
    } else {
        in_max = center;
        out_max = out_max / 2;
    }

    const int run = in_max - in_min;
    if (run == 0) { return 0; }
    const int rise = out_max - out_min;
    const int delta = x - in_min;

    return ((delta * rise) / run) + out_min;
}
