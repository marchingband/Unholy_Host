#include "Arduino.h"
#include <math.h>
#include "constants.h"
#include "config.h"

// dac 1 & 2 : 2238 = 4.935v
// dac 3 : 2911 = 4.935v

#define DAC_1_2_MAX 4095.0
#define DAC_3_MAX 2911.0
#define DAC_V_MAX 4.935

#define NUM_CALIBRATION_POINTS 7

#define MAX_RANGE_INDEX (NUM_CALIBRATION_POINTS - 2)

const uint8_t calibration_values[] = {0, 1, 13, 25, 37, 49, 59};

uint16_t just_dac_vals_v_oct[NUM_DACS][NUM_NOTES_V_OCT];
double volts_hz_v[NUM_DACS][NUM_NOTES_V_OCT];

/*
calibration notes for are:
A-1 | Bb-1 | Bb-2 | Bb-3 | Bb-4 | Bb-5 | Ab-6
*/

double calibration_readings[NUM_DACS][NUM_CALIBRATION_POINTS] = {
    // dac 1 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {
        0.002, // A-1
        0.077, // Bb-1
        1.074, // Bb-2
        2.066, // Bb-3
        3.061, // Bb-4
        4.053, // Bb-5
        4.877, // Ab-6
    },
    // dac 2 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {
        0.001, // A-1
        0.084, // Bb-1
        1.071, // Bb-2
        2.065, // Bb-3
        3.056, // Bb-4
        4.047, // Bb-5
        4.874, // Ab-6
    },
    // dac 3 - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    {
        0.007, // A-1
        0.428, // Bb-1
        0.846, // Bb-2
        1.266, // Bb-3
        1.676, // Bb-4
        2.091, // Bb-5
        2.500, // Ab-6
    },
};

uint16_t volts_to_dac_val(double target_volts, uint8_t num_dac)
{
    double clamped_target_volts = (target_volts > 5.0) ? 5.0 : (target_volts < 0.0) ? 0.0 : target_volts;
    
    // find the right slope
    uint8_t num_range = 0;
    for (int i = 0; i < NUM_CALIBRATION_POINTS; i++)
    {
        if (i == MAX_RANGE_INDEX)
        {
            // no need to check
            num_range = MAX_RANGE_INDEX;
            break;
        }
        double upper_bound_of_range = calibration_readings[num_dac][i + 1];
        if (clamped_target_volts < upper_bound_of_range)
        {
            num_range = i;
            break;
        }
    }
    double dx_min = just_dac_vals_v_oct[num_dac][calibration_values[num_range]];     // the number we sent during calibration
    double dx_max = just_dac_vals_v_oct[num_dac][calibration_values[num_range + 1]]; // the number we sent during calibration, which needs to be trimmed for the last range
    double dy_min = calibration_readings[num_dac][num_range];     // the voltage we got
    double dy_max = calibration_readings[num_dac][num_range + 1]; // the voltage we got
    double dx = dx_max - dx_min;
    double dy = dy_max - dy_min;
    double slope = dx / dy;
    double y_component = clamped_target_volts - dy_min;
    double val = dx_min + (slope * y_component);
    double val_rnd = round(val);
    double val_rnd_clamp = val_rnd < 0.0 ? 0.0 : val_rnd;
    uint16_t out = (uint16_t)val_rnd_clamp;

    double x_component = out - dx_min;
    double inv_slope = dy / dx;

    double expect_volts = dy_min + (x_component * inv_slope);
    double expect_volts_low = dy_min + ((x_component - 1) * inv_slope);
    double expect_volts_high = dy_min + ((x_component + 1) * inv_slope);

    // if (num_dac == 0)
    // {
    //     char log[300];
    //     sprintf(log, "target_volts:%f num_range:%d dx_min:%f dx_max:%f dy_min:%f dy_max:%f slope:%f y_component:%f val:%f val_rnd_clamp:%f, out:%d, expect: %f -> %f <- %f",
    //             target_volts,
    //             num_range,
    //             dx_min,
    //             dx_max,
    //             dy_min,
    //             dy_max,
    //             slope,
    //             y_component,
    //             val,
    //             val_rnd_clamp,
    //             out,
    //             expect_volts_low,
    //             expect_volts,
    //             expect_volts_high);
    //     Serial1.println(log);
    // }

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
    for (int dac = 0; dac < NUM_DACS; dac++)
    {
        if (dac < 2) // dac 1 & 2
        {
            for (int note = 0; note < NUM_NOTES_V_OCT; note++)
            {
                double val = ((double)note * DAC_1_2_MAX) / (12.0 * DAC_V_MAX);
                uint16_t r_val = round(val);
                just_dac_vals_v_oct[dac][note] = r_val;

                // just_dac_vals_v_oct[dac][note] = note * 38;

                // char log[200];
                // sprintf(log, "dac:%d note:%d val:%d", dac, note, just_dac_vals_v_oct[dac][note]);
                // // sprintf(log, "dac:%d note:%d val:%d", dac, note, r_val);
                // Serial1.println(log);
            }
        }
        else // dac 3
        {
            for (int note = 0; note < NUM_NOTES_V_OCT; note++)
            {
                double val = ((double)note * DAC_3_MAX) / (12.0 * DAC_V_MAX);
                just_dac_vals_v_oct[dac][note] = round(val);
            }
        }
    }
}