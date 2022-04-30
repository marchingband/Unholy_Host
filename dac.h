#ifndef DAC_H
#define DAC_H

void dac_init();
void dac_set_chan_one(int val);
void dac_set_chan_two(int val);
void dac_set_chan_all(int val_one, int val_two);

void note_to_dac(uint8_t num_dac, uint8_t note, bool is_v_oct);
void velocity_to_dac(uint8_t num_dac, uint8_t velocity, bool is_v_oct);
void cc_to_dac(uint8_t num_dac, uint8_t value);

void test_dac(void);

#endif