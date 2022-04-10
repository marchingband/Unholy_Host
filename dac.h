#ifndef DAC_H
#define DAC_H

void dac_init();
void dac_set_chan_one(int val);
void dac_set_chan_two(int val);
void dac_set_chan_all(int val_one, int val_two);

#endif