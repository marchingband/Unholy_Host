#ifndef USB_DEVICE_H
#define USB_DEVICE_H

#define MANUFACTURER_ID_1 0x00
#define MANUFACTURER_ID_2 0x21
#define MANUFACTURER_ID_3 0x75 // 117, ascii value for "u" for ultrapalace, should be between 0x60 and 0x7F for sysex spec

#define SYSEX_TYPE_CONFIG 0x00
#define SYSEX_TYPE_ACK 0x01
#define SYSEX_TYPE_REQUEST_CONFIG 0x02
#define SYSEX_TYPE_CAL_MODE_ON 0x03
#define SYSEX_TYPE_CAL_MODE_OFF 0x04

void usb_device_loop(void);
void usb_device_send_config(void);
void send_sysex(const uint8_t *data, size_t size);
void usb_device_send_ack(void);

#endif