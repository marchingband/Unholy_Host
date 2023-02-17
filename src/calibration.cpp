#include "Arduino.h"
#include <math.h>
#include "constants.h"
#include "config.h"

// dac 1 & 2 : 2238 = 4.935v
// dac 3 : 2911 = 4.935v

#define DAC_1_2_MAX 4095.0
// #define DAC_1_2_MAX 2238.0
#define DAC_3_MAX 2911.0
#define DAC_V_MAX 4.935

#define CALIBRATION_INTERVAL 10 // perfect 7th
// #define CALIBRATION_INTERVAL 5 // perfect 4th
#define NUM_CALIBRATION_POINTS (( NUM_NOTES_V_OCT / CALIBRATION_INTERVAL ) + 1 ) // 13
#define MAX_RANGE_INDEX (NUM_CALIBRATION_POINTS - 2)

uint16_t just_dac_vals_v_oct[NUM_DACS][NUM_NOTES_V_OCT];
double volts_hz_v[NUM_DACS][NUM_NOTES_V_OCT];

/*

calibration notes for -- CALIBRATION_INTERVAL = 5 -- are:
A | D | G | C | F | Bb | Eb | Ab | Db | Gb | B | E | Ab  <- note Ab is not a perfect 4th but its the highest note

*/
// double calibration_readings[NUM_DACS][NUM_CALIBRATION_POINTS] = {
//     // dac 1 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     {0.007, 0.424, 0.840, 1.256, 1.670, 2.077, 2.496, 2.913, 3.330, 3.742, 4.156, 4.574, 4.903},
//     // {0.001, 0.414, 0.827, 1.245, 1.664, 2.078, 2.495, 2.911, 3.331, 3.752, 4.168, 4.587, 4.921},
//     // dac 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     {0.006, 0.410, 0.829, 1.242, 1.659, 2.069, 2.487, 2.889, 3.315, 3.725, 4.140, 4.555, 4.882},
//     // {0.005, 0.432, 0.835, 1.264, 1.673, 2.100, 2.509, 2.932, 3.345, 3.762, 4.176, 4.595, 4.929},
//     // dac 3 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//     {0.007, 0.428, 0.846, 1.266, 1.676, 2.091, 2.500, 2.912, 3.329, 3.742, 4.165, 4.579, 4.913},
// };
/*

calibration notes for -- CALIBRATION_INTERVAL = 10 -- are:
A | G | F | Eb | Db | B | Ab  <- note Ab is not a perfect 4th but its the highest note

// */
double calibration_readings[NUM_DACS][NUM_CALIBRATION_POINTS] = {
    // dac 1 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.001, 0.823, 1.652, 2.480, 3.307, 4.134, 4.877, },
    // dac 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.001, 0.828, 1.648, 2.477, 3.302, 4.129, 4.875, },
    // dac 3 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {0.007, 0.428, 0.846, 1.266, 1.676, 2.091, 2.500, },
};

uint16_t volts_to_dac_val(double target_volts, uint8_t num_dac)
{
    double clamped_target_volts = (target_volts > 5.0) ? 5.0 : (target_volts < 0.0) ? 0.0 : target_volts;
    // find the right slope
    uint8_t num_range = 0;
    for(int i=0; i<NUM_CALIBRATION_POINTS; i++)
    {
        if(i == MAX_RANGE_INDEX)
        {
            // no need to check
            num_range = MAX_RANGE_INDEX;
            break;
        }
        double upper_bound_of_range = calibration_readings[num_dac][i + 1];
        if(clamped_target_volts < upper_bound_of_range)
        {
            num_range = i;
            break;
        }
    }
    double dx_min = just_dac_vals_v_oct[num_dac][num_range * CALIBRATION_INTERVAL]; // the number we sent during calibration
    double dx_max = just_dac_vals_v_oct[num_dac][(( num_range + 1 ) * CALIBRATION_INTERVAL) - (num_range >= MAX_RANGE_INDEX)]; // the number we sent during calibration, which needs to be trimmed for the last range
    double dy_min = calibration_readings[num_dac][num_range]; // the voltage we got
    double dy_max = calibration_readings[num_dac][num_range + 1]; // the voltage we got
    double dx = dx_max - dx_min;
    double dy = dy_max - dy_min;
    double slope = dx / dy;
    double y_component = clamped_target_volts - dy_min;
    double val = dx_min + ( slope * y_component);
    double val_rnd = round(val);
    double val_rnd_clamp = val_rnd < 0.0 ? 0.0 : val_rnd;
    uint16_t out = (uint16_t)val_rnd_clamp;

    double x_component = out - dx_min;
    double inv_slope = dy / dx;

    double expect_volts = dy_min + (x_component * inv_slope);
    double expect_volts_low = dy_min + ((x_component - 1) * inv_slope);
    double expect_volts_high = dy_min + ((x_component + 1) * inv_slope);


    if(num_dac == 0)
    {
        char log[300];
        sprintf(log, "target_volts:%f num_range:%d dx_min:%f dx_max:%f dy_min:%f dy_max:%f slope:%f y_component:%f val:%f val_rnd_clamp:%f, out:%d, expect: %f -> %f <- %f", 
            target_volts,
            num_range,
            dx_min,
            dx_max,
            dy_min,
            dy_max,
            slope,
            y_component,
            val,
            val_rnd_clamp,
            out,
            expect_volts_low,
            expect_volts,
            expect_volts_high
        );
        Serial1.println(log);
    }

    // char log[200];
    // sprintf(log, "val:%f out:%d", val, out );
    // Serial1.println(log);

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
                uint16_t r_val = round(val);
                just_dac_vals_v_oct[dac][note] = r_val;

                // just_dac_vals_v_oct[dac][note] = note * 38;

                char log[200];
                sprintf(log, "dac:%d note:%d val:%d", dac, note, just_dac_vals_v_oct[dac][note]);
                // sprintf(log, "dac:%d note:%d val:%d", dac, note, r_val);
                Serial1.println(log);
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