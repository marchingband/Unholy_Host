#ifndef USB_DEVICE_H
#define USB_DEVICE_H

void usb_device_loop(void);
void usb_device_send_config(void);
void send_sysex(const uint8_t *data, size_t size);

#endif