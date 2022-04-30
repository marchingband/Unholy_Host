#ifndef CALIBRATION_H
#define CALIBRATION_H

uint16_t note_to_dac_val_calibration(uint8_t note, uint8_t num_dac);
uint16_t volts_to_dac_val(double target_volts, uint8_t num_dac);
void init_just_dac_vals(void);

#endif