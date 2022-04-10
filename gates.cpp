#include <ShiftRegister74HC595.h>
#include "Arduino.h"

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
