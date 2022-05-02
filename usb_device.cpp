#include "Arduino.h"
#include <usbmidi.h>
#include "midi.h"

void usb_device_loop(void)
{
    USBMIDI.poll();
    while (USBMIDI.available()) {
		// We must read entire available data, so in case we receive incoming
		// MIDI data, the host wouldn't get stuck.
		// Serial1.println("usb_device_loop");
		unsigned char b = USBMIDI.read();
		midi_device_parse(b);
	}
    USBMIDI.flush();
}