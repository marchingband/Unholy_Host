#include <MCP48xx.h>
#include "Arduino.h"
#include <math.h>
#include "constants.h"
#include "calibration.h"
#include "config.h"

// #define CALIBRATION_MODE
// #define DEBUG
#define NUM_NOTES_HZ_V (HZ_V_MAX - HZ_V_MIN)   // 63 // A0 to G#4

MCP4822 *dac;

int chan_one = 0;
int chan_two = 0;

double volts_hz_v_lut[NUM_NOTES_HZ_V];

void init_hz_v_lut(void)
{
    volts_hz_v_lut[0] = 0.0;         // place the 1st hz/v
    volts_hz_v_lut[1] = 0.125 / 2; // place the 2nd hz/v
    for(int note=2; note<NUM_NOTES_HZ_V; note++)
    {
        double val = volts_hz_v_lut[note - 1]  * pow(2.0, 1.0 / 12.0);
        volts_hz_v_lut[note] = val;
    }
}

void dac_init()
{
    dac = new MCP4822(1);
    dac->init();
    dac->turnOnChannelA();
    dac->turnOnChannelB();
    // dac->setGainA(MCP4822::Low);
    // dac->setGainB(MCP4822::Low);
    dac->setGainA(MCP4822::High);
    dac->setGainB(MCP4822::High);
    pinMode(0, OUTPUT);
    analogWriteResolution(10);
    init_hz_v_lut();
}

void dac_set_chan_one(int val)
{
    chan_one = val;
    dac->setVoltageA(chan_one);
    dac->updateDAC();
}

void dac_set_chan_two(int val)
{
    chan_two = val;
    dac->setVoltageB(chan_two);
    dac->updateDAC();
}

void dac_set_chan_three(int val)
{
    analogWrite(0, map(val, 0, 4095, 0, 1023)); // 0->3.3v with 0->1023 (10 bit) value
}

void dac_set_chan_all(int val_one, int val_two)
{
    chan_one = val_one;
    chan_two = val_two;
    dac->setVoltageA(chan_one);
    dac->setVoltageB(chan_two);
    dac->updateDAC();
}

uint16_t midi_to_v_oct(uint8_t num_dac, uint8_t note)
{
#ifdef CALIBRATION_MODE
    return note_to_dac_val_calibration(note, num_dac);
#else
    int clamped_note = note > V_OCT_MAX ? V_OCT_MAX : note < V_OCT_MIN ? V_OCT_MIN : note;
    int index = clamped_note - V_OCT_MIN;
    double volts = (double)(index) / (double)(12.0);
    return volts_to_dac_val(volts, num_dac);
#endif
}

uint16_t midi_to_hz_v(uint8_t num_dac, uint8_t note)
{
    int clamped_note = note > HZ_V_MAX ? HZ_V_MAX : note < HZ_V_MIN ? HZ_V_MIN : note;
    int index = clamped_note - HZ_V_MIN;
    double volts = volts_hz_v_lut[index];
    return volts_to_dac_val(volts, num_dac);
}

void note_to_dac(uint8_t num_dac, uint8_t note, bool is_v_oct)
{
    char log[20];
    sprintf(log, "dac%d note-%d", num_dac, note);
    Serial1.println(log);

    uint16_t val = is_v_oct ? midi_to_v_oct(num_dac, note) : midi_to_hz_v(num_dac, note);

    if(num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if(num_dac == 1)
    {
        dac_set_chan_two(val);
    }
    else
    {
        dac_set_chan_three(val);
    }
}

void velocity_to_dac(uint8_t num_dac, uint8_t velocity)
{
    double velocity_d = (double)velocity;
    double volts = (velocity_d * 4.935) / 127.0;
    uint16_t val = volts_to_dac_val(volts, num_dac);

    // char log1[100];
    // sprintf(log1, "velocity_to_dac velocity:%d val:%d volts:%f velocity_d:%f", velocity ,val, volts, velocity_d);
    // Serial1.println(log1);

    if(num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if(num_dac == 1)
    {
        dac_set_chan_two(val);
    }
    else
    {
        dac_set_chan_three(val);
    }
}

void cc_to_dac(uint8_t num_dac, uint8_t value)
{
    double value_d = (double)value;
    double volts = (value_d * 4.935) / 127.0;
    uint16_t val = volts_to_dac_val(volts, num_dac);

    // char log1[100];
    // sprintf(log1, "cc_to_dac value:%d val:%d volts:%f value_d:%f", value ,val, volts, value_d);
    // Serial1.println(log1);
    
    if(num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if(num_dac == 1)
    {
        dac_set_chan_two(val);
    }
    else
    {
        dac_set_chan_three(val);
    }
}

void test_dac(void)
{
    int base = 2230;
    for(int i=0; i< 100; i++)
    {
        // Serial1.println(base + i);
        // dac_set_chan_one(base + i);
        // dac_set_chan_two(base + i);
        // dac_set_chan_three(base + i);
        dac_set_chan_all(1000,1000);
        while(1)
        {
            delay(1000);
        }
    }
}