#include "midi_interface.h"



#define DEFAULT_MIDI_CHANNEL 10
#define DEFAULT_VELOCITY 102
#define SILENCE 0
#define PB 128

// this map is upside down
const byte BUTTON_MAP[MAX_BUTTONS] = {
    48, 46, 38, 45,
    36, 37, 41, 40,
    39, 47, 44, 49,
    43, 42, 50, 51
};
const byte POT_CC[MAX_POTS] = { 21, 22, 23 };
const byte JS_CC[MAX_AXES] = { PB, 1 };

byte midi_channel = DEFAULT_MIDI_CHANNEL;
byte velocity = DEFAULT_VELOCITY;

// private declations:
int midi_map(int x, int in_min, int in_max, int out_min, int out_max);



MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midi2);



void midi_setup()
{
    midi2.begin(midi_channel);
}



void midi_sendNoteOn(int b)
{
    midi2.sendNoteOn(BUTTON_MAP[b], velocity, midi_channel);

    activity_monitor(1);
}



void midi_sendNoteOff(int b)
{
    midi2.sendNoteOff(BUTTON_MAP[b], SILENCE, midi_channel);
    // midi2.sendNoteOn(note, SILENCE, midi_channel);

    activity_monitor(1);
}



void midi_sendCC_pot(int pot, int value)
{
    byte cc_value = (byte)midi_map(value, 0, 4095, 0, 127);
    midi2.sendControlChange(POT_CC[pot], cc_value, midi_channel);

    activity_monitor(1);
}



void midi_sendCC_js(int axis, int value)
{
    int minval, maxval;
    if (axis == 0) {
        minval = 0;
        maxval = 4095;
    } else {
        minval = 4095;
        maxval = 0;
    }

    if (JS_CC[axis] == PB) {
        int pb_value = midi_map(value, minval, maxval, MIDI_PITCHBEND_MIN, MIDI_PITCHBEND_MAX);
        midi2.sendPitchBend(pb_value, midi_channel);
    } else {
        byte cc_value = (byte)midi_map(value, minval, maxval, 0, 127);
        midi2.sendControlChange(JS_CC[axis], cc_value, midi_channel);
    }

    activity_monitor(1);
}



int midi_map(int x, int in_min, int in_max, int out_min, int out_max)
{
    const int run = in_max - in_min;
    if (run == 0) { return 0; }
    const int rise = out_max - out_min;
    const int delta = x - in_min;

    return ((delta * rise) / run) + out_min;
}
