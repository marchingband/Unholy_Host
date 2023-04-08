#include "Arduino.h"
#include "MIDIUSB.h"
#include "midi.h"
#include "usb_device.h"
#include "config.h"

midiEventPacket_t rx;
uint8_t midi_buf[128];

void usb_device_loop(void)
{
	do 
	{
		rx = MidiUSB.read();
		if (rx.header != 0) 
		{
			// midi_device_parse(rx.header);
			midi_device_parse(rx.byte1);
			midi_device_parse(rx.byte2);
			midi_device_parse(rx.byte3);
		}
	} 
	while (rx.header != 0);
}

const uint8_t buffer[] = {0xF0, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xF7};

void usb_device_send_config(void)
{
    //config is 132 bytes (add 2 for systex header and footer)
	uint8_t config_array[134] = {0};
    config_array[0] = 0xF0;
    encode_config(&config_array[1]);
    config_array[133] = 0xF7;
    send_sysex(config_array, 134);
    // uint8_t test[] = {1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0};
    // send_sysex(test , 20);
}

void send_sysex(const uint8_t *data, size_t size)
{
    if (data == NULL || size == 0) return;

    size_t midiDataSize = (size+2)/3*4;
    uint8_t midiData[midiDataSize];
    const uint8_t *d = data;
    uint8_t *p = midiData;
    size_t bytesRemaining = size;

    while (bytesRemaining > 0) {
        switch (bytesRemaining) {
        case 1:
            *p++ = 5;   // SysEx ends with following single byte
            *p++ = *d;
            *p++ = 0;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 2:
            *p++ = 6;   // SysEx ends with following two bytes
            *p++ = *d++;
            *p++ = *d;
            *p = 0;
            bytesRemaining = 0;
            break;
        case 3:
            *p++ = 7;   // SysEx ends with following three bytes
            *p++ = *d++;
            *p++ = *d++;
            *p = *d;
            bytesRemaining = 0;
            break;
        default:
            *p++ = 4;   // SysEx starts or continues
            *p++ = *d++;
            *p++ = *d++;
            *p++ = *d++;
            bytesRemaining -= 3;
            break;
        }
    }
    MidiUSB.write(midiData, midiDataSize);
}