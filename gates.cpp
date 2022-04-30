#include <ShiftRegister74HC595.h>
#include "Arduino.h"

#define TRIG_TIME_MILLIS 10

// parameters: <number of shift registers> (data pin, clock pin, latch pin)
ShiftRegister74HC595<1> *sr;

void gates_init()
{
    sr = new ShiftRegister74HC595<1>(2, 3, 4);
}

void gate_set(uint8_t pin, const uint8_t value)
{
    sr->set(pin, value);
}

uint8_t gate_get(uint8_t pin)
{
    sr->get(pin);
}

unsigned long gate_eta[8] = {0,0,0,0,0,0,0,0};

void trigger_start(uint8_t num_trig)
{
    gate_eta[num_trig] = millis() + TRIG_TIME_MILLIS;
    gate_set(num_trig, 1);
}

void clear_triggers(void)
{
    unsigned long now = millis();
    for(int i=0;i<8;i++)
    {
        if(
            (gate_eta[i] > 0) &&
            (gate_eta[i] < now)
        )
        {
            gate_set(i, 0);
            gate_eta[i] = 0;
        }
    }
}
