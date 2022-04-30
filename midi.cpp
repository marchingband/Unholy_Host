#include <Arduino.h>
#include "midiXparser.h"
#include "midi.h"
#include "dac.h"
#include "gates.h"
#include "cv.h"
#include "handlers.h"
#include "config.h"
#include "constants.h"

#define DAC_1 0
#define DAC_2 1
#define DAC_3 2

midiXparser midiHostParser;
midiXparser midiDeviceParser;

int hi = 0;

void handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    char log[100];
    sprintf(log, "note-on chan:%d note:%d vel:%d", channel, note, velocity);
    Serial1.println((const char *)log);
    cv_1_handle_note_on_off(channel, note, velocity, is_note_on);
    cv_2_handle_note_on_off(channel, note, velocity, is_note_on);
    cv_3_handle_note_on_off(channel, note, velocity, is_note_on);
    duophonic_handle_note_on_off(channel, note, velocity, is_note_on);
    triphonic_handle_note_on_off(channel, note, velocity, is_note_on);
    gates_handle_note_on_off(channel, note, velocity, is_note_on);
}

void handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
    char log[100];
    sprintf(log, "cc chan:%d cc:%d val:%d", channel, cc, val);
    Serial1.println((const char *)log);
    cv_1_handle_cc(channel, cc, val);
    cv_2_handle_cc(channel, cc, val);
    cv_3_handle_cc(channel, cc, val);
    gates_handle_cc(channel, cc, val);
}

void handle_clock(uint8_t channel)
{
    char log[100];
    sprintf(log, "clock chan:%d", channel);
    Serial1.println((const char *)log);
    gates_handle_clock(channel);
}

void handle_transport(uint8_t channel, uint8_t code)
{
    char log[100];
    sprintf(log, "transport chan:%d %s", channel, code == MIDI_START ? "start" : code == MIDI_CONTINUE ? "continue" : "stop");
    Serial1.println((const char *)log);
    gates_handle_transport(channel, code);
}

void handle_midi(uint8_t *msg)
{
    // blink
    digitalWrite(13, hi);
    hi = hi==1 ? 0 : 1;

    if (!msg) return;
    uint8_t channel = msg[0] & 0b00001111;
    uint8_t code = (msg[0] >> 4) & 0b00001111;
    switch(code){
        case MIDI_NOTE_ON:
        {
            uint8_t note = msg[1] & 0b01111111;
            uint8_t velocity = msg[2] & 0b01111111;
            handle_note_on_off(channel + 1, note, velocity, true);
            break;
        }
        case MIDI_NOTE_OFF:
        {
            uint8_t note = msg[1] & 0b01111111;
            uint8_t velocity = msg[2] & 0b01111111;
            handle_note_on_off(channel + 1, note, 0, false);
            break;
        }
        case MIDI_CC:
        {
            uint8_t cc = msg[1] & 0b01111111;
            uint8_t val = msg[2]  & 0b01111111;
            handle_cc(channel + 1, cc, val);
            break;
        }
        case MIDI_CLOCK:
        {
            handle_clock(channel);
        }
        case MIDI_START:
        case MIDI_STOP:
        case MIDI_CONTINUE:
        {
            handle_transport(channel, code);
        }
    }
}

void midi_host_parse(uint8_t in)
{
    if ( midiHostParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        uint8_t *msg = midiHostParser.getMidiMsg();
        handle_midi(msg);
    }
}

void midi_device_parse(uint8_t in)
{
    if ( midiDeviceParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        uint8_t *msg = midiDeviceParser.getMidiMsg();
        handle_midi(msg);
    }
}

void midi_parser_init(void)
{
    midiHostParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk );
    midiDeviceParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk );
}