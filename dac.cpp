#include <MCP48xx.h>
#include "Arduino.h"

MCP4822 *dac;

int chan_one = 0;
int chan_two = 0;

void dac_init()
{
    dac = new MCP4822(1);
    dac->init();
    dac->turnOnChannelA();
    dac->turnOnChannelB();
    dac->setGainA(MCP4822::High);
    dac->setGainB(MCP4822::High);
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

void dac_set_chan_all(int val_one, int val_two)
{
    chan_one = val_one;
    chan_two = val_two;
    dac->setVoltageA(chan_one);
    dac->setVoltageB(chan_two);
    dac->updateDAC();
}