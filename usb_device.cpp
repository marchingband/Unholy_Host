#include "Arduino.h"
#include "MIDIUSB.h"
#include "midi.h"

midiEventPacket_t rx;
uint8_t midi_buf[128];

void usb_device_loop(void)
{
	do 
	{
		rx = MidiUSB.read();
		if (rx.header != 0) 
		{
			midi_device_parse(rx.header);
			midi_device_parse(rx.byte1);
			midi_device_parse(rx.byte2);
			midi_device_parse(rx.byte3);
		}
	} 
	while (rx.header != 0);
}