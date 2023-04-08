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

#define MSG_LEN 138

void usb_device_send_config(void)
{
    //config is 132 bytes (add 2 for systex header and footer, 3 for device ids, 1 for sysex type)
	uint8_t config_array[MSG_LEN] = {0};
    config_array[0] = 0xF0;
    config_array[1] = MANUFACTURER_ID_1;
    config_array[2] = MANUFACTURER_ID_2;
    config_array[3] = MANUFACTURER_ID_3;
    config_array[4] = SYSEX_TYPE_CONFIG;
    encode_config(&config_array[5]);
    config_array[MSG_LEN - 1] = 0xF7;
    send_sysex(config_array, MSG_LEN);
}

void usb_device_send_ack(void)
{
    uint8_t ack_array[6];
    ack_array[0] = 0xF0;
    ack_array[1] = MANUFACTURER_ID_1;
    ack_array[2] = MANUFACTURER_ID_2;
    ack_array[3] = MANUFACTURER_ID_3;
    ack_array[4] = SYSEX_TYPE_ACK;
    ack_array[5] = 0xF7;
    send_sysex(ack_array, 6);
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