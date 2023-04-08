#include <MCP48xx.h>
#include "Arduino.h"
#include <math.h>
#include "constants.h"
#include "calibration.h"
#include "config.h"
#include "midi.h"
#include "handlers.h"

MCP4822 *dac;

double volts_hz_v_lut[NUM_NOTES_HZ_V];

extern struct config_t *config;

void init_hz_v_lut(void)
{
    volts_hz_v_lut[0] = 0.0;       // place the 1st hz/v
    volts_hz_v_lut[1] = 0.125 / 2; // place the 2nd hz/v
    for (int note = 2; note < NUM_NOTES_HZ_V; note++)
    {
        double val = volts_hz_v_lut[note - 1] * pow(2.0, 1.0 / 12.0);
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
    // pinMode(0, OUTPUT); // this must not be called
    analogWriteResolution(10);
    init_hz_v_lut();
}

void dac_set_chan_one(int val)
{
    // Serial1.println(val);
    dac->setVoltageA(val);
    dac->updateDAC();
}

void dac_set_chan_two(int val)
{
    // Serial1.println(val);
    dac->setVoltageB(val);
    dac->updateDAC();
}

void dac_set_chan_three(int val)
{
    int out_val = map(val, 0, 4095, 0, 1023);
    out_val = out_val > 1023 ? 1023 : out_val;
    // analogWrite(A0, 1023);
    analogWrite(0, out_val);
    // analogWrite(0, map(val, 0, 4095, 0, 1023)); // 0->3.3v with 0->1023 (10 bit) value
    Serial1.println(val);
    Serial1.println(out_val);
}

void dac_set_chan_all(int val_one, int val_two)
{
    dac->setVoltageA(val_one);
    dac->setVoltageB(val_two);
    dac->updateDAC();
}

uint16_t midi_to_v_oct(uint8_t num_dac, uint8_t note)
{
    int clamped_note = (note > V_OCT_MAX) ? V_OCT_MAX : (note < V_OCT_MIN) ? V_OCT_MIN
                                                                           : note;
    int index = clamped_note - V_OCT_MIN;

#ifdef CALIBRATION_MODE
    return note_to_dac_val_calibration(index, num_dac);
#else

    double volts = (double)(index) / (double)(12.0);

    // char log[200];
    // sprintf(log, "midi_to_v_oct note:%d clamped_note:%d index:%d volts:%f", note, clamped_note, index, volts);
    // Serial1.println(log);

    // calculate pitch bend
    double pitch_bend_range_volts = config->PITCH_BEND_RANGE / 12.0;
    double pitch_state = (double)pitch_bend_state;
    double pitch_state_signed = pitch_state - 8192.0;
    double pitch_ratio = pitch_state_signed / 8192.0;
    double pitch_bend = pitch_bend_range_volts * pitch_ratio;
    double bent_volts = volts + pitch_bend;

    // char log1[200];
    // sprintf(log1, "pitch_bend_range_volts:%f pitch_state:%f pitch_state_signed:%f pitch_ratio:%f pitch_bend:%f volts:%f bent_volts:%f\n", 
    //     pitch_bend_range_volts, 
    //     pitch_state, 
    //     pitch_state_signed, 
    //     pitch_ratio, 
    //     pitch_bend, 
    //     volts, 
    //     bent_volts);
    // Serial1.println(log1);

    return volts_to_dac_val(bent_volts, num_dac);
#endif
}

uint16_t midi_to_hz_v(uint8_t num_dac, uint8_t note)
{
    int clamped_note = note > HZ_V_MAX ? HZ_V_MAX : note < HZ_V_MIN ? HZ_V_MIN
                                                                    : note;
    int index = clamped_note - HZ_V_MIN;
    double volts = volts_hz_v_lut[index];

    double pitch_bend_range_volts = config->PITCH_BEND_RANGE / 12.0;
    double pitch_state = (double)pitch_bend_state;
    double pitch_state_signed = pitch_state - 8192.0;
    double pitch_ratio = pitch_state_signed / 8192.0;
    double pitch_bend = pitch_bend_range_volts * pitch_ratio;
    double frequency_factor = pow(2.0, pitch_bend);
    double bent_volts = volts * frequency_factor;

    // char log[200];
    // sprintf(log, "pitch_bend_range_volts:%f pitch_state:%f pitch_state_signed:%f pitch_ratio:%f pitch_bend:%f frequency_factor:%f volts:%f bent_volts:%f", pitch_bend_range_volts, pitch_state, pitch_state_signed, pitch_ratio, pitch_bend, frequency_factor, volts, bent_volts);
    // Serial1.println(log);

    return volts_to_dac_val(bent_volts, num_dac);
}

void note_to_dac(uint8_t num_dac, uint8_t note, bool is_v_oct)
{
    // char log[20];
    // sprintf(log, "dac%d note-%d", num_dac, note);
    // Serial1.println(log);

    // update state
    cvs[num_dac].note = note;

#ifdef CALIBRATION_MODE
    uint16_t val = midi_to_v_oct(num_dac, note); // calibration is always v/oct
#else
    uint16_t val = is_v_oct ? midi_to_v_oct(num_dac, note) : midi_to_hz_v(num_dac, note);
#endif

    if (num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if (num_dac == 1)
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

    if (num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if (num_dac == 1)
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

    if (num_dac == 0)
    {
        dac_set_chan_one(val);
    }
    else if (num_dac == 1)
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
    for (int i = 1000; i < 1100; i++)
    {
        dac_set_chan_one(1 * i);

        // Serial1.println(base + i);
        // dac_set_chan_one(base + i);
        // dac_set_chan_two(base + i);
        // dac_set_chan_three(base + i);
        // dac_set_chan_all(1000,1000);
        delay(1000);
        // while(1)
        // {
        // }
    }
}