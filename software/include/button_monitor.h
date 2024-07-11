#pragma once

#include <Arduino.h>

#include "midi_interface.h"



#define MAX_BUTTONS 16

void button_setup(void);
void button_handler(void);
