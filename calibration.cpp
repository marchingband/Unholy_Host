#include "Arduino.h"
#include <math.h>
#include "constants.h"
#include "config.h"

// dac 1 & 2 : 2238 = 4.935v
// dac 3 : 2911 = 4.935v

#define DAC_1_2_MAX 2238.0
#define DAC_3_MAX 2911.0
#define DAC_V_MAX 4.935

#define NUM_NOTES_V_OCT_ (V_OCT_MAX - V_OCT_MIN)// 60 // A0 to G#4
#define CALIBRATION_INTERVAL 5 // perfect 4th
#define NUM_CALIBRATION_POINTS (( NUM_NOTES_V_OCT / CALIBRATION_INTERVAL ) + 1 ) // 13

uint16_t just_dac_vals_v_oct[NUM_DACS][NUM_NOTES_V_OCT];
double volts_hz_v[NUM_DACS][NUM_NOTES_V_OCT];

double calibration_readings[NUM_DACS][NUM_CALIBRATION_POINTS] = {
    // dac 1 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.001, 0.414, 0.827, 1.245, 1.664, 2.078, 2.495, 2.911, 3.331, 3.752, 4.168, 4.587, 4.935},
    // dac 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.005, 0.432, 0.835, 1.263, 1.672, 2.099, 2.508, 2.931, 3.344, 3.761, 4.174, 4.594, 4.935},
    // dac 3 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.009, 0.430, 0.846, 1.266, 1.676, 2.090, 2.499, 2.910, 3.327, 3.739, 4.161, 4.574, 4.935},
};

uint16_t volts_to_dac_val(double target_volts, uint8_t num_dac)
{
    // find the right slope - - - - - - - - - - - - - - - - - - - -- - - - - - - - - - - - - - - - -
    uint8_t num_range = 0;
    for(int i=0; i<NUM_CALIBRATION_POINTS; i++)
    {
        if(i == ( NUM_CALIBRATION_POINTS - 1))
        {
            // no need to check - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
            num_range = (NUM_CALIBRATION_POINTS - 1);
            break;
        }
        double upper_bound_of_range = calibration_readings[num_dac][i + 1];
        if(target_volts < upper_bound_of_range)
        {
            num_range = i;
            break;
        }
    }
    double dx_min = just_dac_vals_v_oct[num_dac][num_range * CALIBRATION_INTERVAL]; // the number we sent during calibration
    double dx_max = just_dac_vals_v_oct[num_dac][( num_range + 1 ) * CALIBRATION_INTERVAL]; // the number we sent during calibration
    double dy_min = calibration_readings[num_dac][num_range]; // the voltage we got
    double dy_max = calibration_readings[num_dac][num_range + 1]; // the voltage we got
    double dx = dx_max - dx_min;
    double dy = dy_max - dy_min;
    double slope = dx / dy;
    double y_component = target_volts - dy_min;
    double val = dx_min + ( slope * y_component);
    double val_rnd = round(val);
    val_rnd = val_rnd < 0.0 ? 0.0 : val_rnd; // clamp to 0 
    uint16_t out = (uint16_t)val_rnd;
    return out;
}

uint16_t note_to_dac_val_calibration(uint8_t note, uint8_t num_dac)
{
    return just_dac_vals_v_oct[num_dac][note];
}

void init_just_dac_vals(void)
{
    for(int dac=0; dac<NUM_DACS; dac++)
    {
        if(dac<2) // dac 1 & 2
        {
            for(int note=0; note<NUM_NOTES_V_OCT; note++)
            {
                double val = ((double)note * DAC_1_2_MAX) / (12.0 * DAC_V_MAX);
                just_dac_vals_v_oct[dac][note] = round(val);
            }
        }
        else // dac 3
        {
            for(int note=0; note<NUM_NOTES_V_OCT; note++)
            {
                double val = ((double)note * DAC_3_MAX) / (12.0 * DAC_V_MAX);
                just_dac_vals_v_oct[dac][note] = round(val);
            }
        }
    }
}