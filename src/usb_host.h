#ifndef USB_MIDI_HOST
#define USB_MIDI_HOST

void usb_host_init(void);
void usb_host_loop(void);
void CUSTOM_UHD_Handler(void);
void handleBank0(uint32_t epAddr);
void handleBank1(uint32_t epAddr);

#endif
