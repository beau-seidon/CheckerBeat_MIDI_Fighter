#include "pot_monitor.h"



const int POT_PINS[MAX_POTS] = { 33, 25, 32 };
const int POT_DEADBAND = (4096 / 128) - 1;
const int POT_CHANGE_DELAY = 5;  // ms

int active_pot = 0;

int pot_values[MAX_POTS] = { 0 };
int prev_pot_values[MAX_POTS] = { 0 };

static unsigned long T_act = millis();         // active-pot change timer
unsigned long T_pot[MAX_POTS] = { millis() };  // pot debounce timers

// private declarations:
void read_pot(int pot);
void pots_to_midi(int pot);



void pot_setup()
{
    pinMode(POT_PINS[0], INPUT);  // Velocity
    pinMode(POT_PINS[1], INPUT);  // CC#20
    pinMode(POT_PINS[2], INPUT);  // CC#21

    for (int p = 0; p < MAX_POTS; ++p) { prev_pot_values[p] = pot_values[p]; }
}



void pot_handler()
{
    unsigned long T = millis();
    if (T > T_act + POT_CHANGE_DELAY) {
        T_act = T;
        ++active_pot;
        if (active_pot >= MAX_POTS) active_pot = 0;
    }

    read_pot(active_pot);
    pots_to_midi(active_pot);
}



void read_pot(int pot)
{
    int pot_raw_value = analogRead(POT_PINS[pot]);  // charge the ADC
    pot_raw_value = analogRead(POT_PINS[pot]);
    if (abs(pot_values[pot] - pot_raw_value) > POT_DEADBAND) {  // prevent ADC jitter and noise
        pot_values[pot] = pot_raw_value;
    }
}



void pots_to_midi(int pot)
{
    if(pot_values[pot] != prev_pot_values[pot]) {
        prev_pot_values[pot] = pot_values[pot];
        midi_sendCC_pot(pot, pot_values[pot]);
    }
}
