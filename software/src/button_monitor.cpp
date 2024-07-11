#include "button_monitor.h"



// SN74HC165 connections:
#define LOAD_PIN 12     // SH/LD (DIP PIN 1)
#define CLK_PIN 14      // CLK (DIP PIN 2)
#define DATA_PIN 27     // QH' (DIP PIN 7)
#define CLK_INH_PIN 26  // CLK INH (DIP PIN 15)

const int DEBOUNCE_TIME = 20;  // ms

// 2 bytes to hold 16 button states
byte buttons1 = 0x0;
byte buttons2 = 0x0;

byte button_pressed[MAX_BUTTONS] = { 0 };

unsigned long T_btn[MAX_BUTTONS] = { millis() };  // button debounce timers

// private declarations:
void read_shift_register(void);
bool debounce(int b);
void buttons_to_midi(byte shiftreg1, byte shiftreg2);



void button_setup()
{
    pinMode(LOAD_PIN, OUTPUT);
    pinMode(CLK_INH_PIN, OUTPUT);
    pinMode(CLK_PIN, OUTPUT);
    pinMode(DATA_PIN, INPUT);
}



void button_handler()
{
    read_shift_register();
    buttons_to_midi(~buttons1, ~buttons2);
}



void read_shift_register()
{
    digitalWrite(LOAD_PIN, LOW);
    delayMicroseconds(5);

    digitalWrite(LOAD_PIN, HIGH);
    delayMicroseconds(5);

    digitalWrite(CLK_PIN, HIGH);
    digitalWrite(CLK_INH_PIN, LOW);
    digitalWrite(CLK_PIN, LOW);

    buttons1 = shiftIn(DATA_PIN, CLK_PIN, MSBFIRST);
    buttons2 = shiftIn(DATA_PIN, CLK_PIN, MSBFIRST);

    digitalWrite(CLK_INH_PIN, HIGH);
}



bool debounce(int b)
{
    if (millis() > (T_btn[b] + DEBOUNCE_TIME)) { return true; }
    return false;
}



void buttons_to_midi(byte shiftreg1, byte shiftreg2)
{
    for(int b = 0; b < MAX_BUTTONS; ++b) {  // b++?
        if(b < 8) {
            if (shiftreg1 & 0x01 == 1) {
                if(!button_pressed[b] && debounce(b)) {
                    button_pressed[b] = 1;
                    midi_sendNoteOn(b);
                    T_btn[b] = millis();
                }
            } else {
                if(button_pressed[b] && debounce(b)) {
                    button_pressed[b] = 0;
                    midi_sendNoteOff(b);
                    T_btn[b] = millis();
                }
            }
            shiftreg1 = shiftreg1 >> 1;
        }
        if(b > 7) {
            if (shiftreg2 & 0x01 == 1) {
                if(!button_pressed[b] && debounce(b)) {
                    button_pressed[b] = 1;
                    midi_sendNoteOn(b);
                    T_btn[b] = millis();
                }
            } else {
                if(button_pressed[b] && debounce(b)) {
                    button_pressed[b] = 0;
                    midi_sendNoteOff(b);
                    T_btn[b] = millis();
                }
            }
            shiftreg2 = shiftreg2 >> 1;
        }
    }
}
