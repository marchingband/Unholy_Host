#ifndef MIDI_H
#define MIDI_H

void midi_host_parse(uint8_t in);
void midi_device_parse(uint8_t in);
void midi_parser_init(void);

#endif