#pragma once

#include <Arduino.h>
#include <MIDI.h>

#include "activity_monitor.h"
#include "button_monitor.h"
#include "pot_monitor.h"
#include "joystick_monitor.h"



void midi_setup(void);
void midi_handler(void);
void midi_sendNoteOn(int note);
void midi_sendNoteOff(int note);
void midi_sendCC_pot(int pot, int value);
void midi_sendCC_js(int axis, int value);
