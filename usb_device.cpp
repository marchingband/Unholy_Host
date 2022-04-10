#include <usbmidi.h>
#include "midi.h"

void usb_device_init(void)
{

}

void usb_device_loop(void)
{
    USBMIDI.poll();
    while (USBMIDI.available()) {
		// We must read entire available data, so in case we receive incoming
		// MIDI data, the host wouldn't get stuck.
		unsigned char b = USBMIDI.read();
		midi_device_parse(b);
	}
    USBMIDI.flush();
}