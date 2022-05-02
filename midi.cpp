#include <Arduino.h>
#include "midiXparser.h"
#include "midi.h"
#include "dac.h"
#include "gates.h"
#include "handlers.h"
#include "config.h"
#include "constants.h"

#define DAC_1 0
#define DAC_2 1
#define DAC_3 2

midiXparser midiHostParser;
midiXparser midiDeviceParser;

int hi = 0;

uint8_t *msg;

void handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    // char log[100];
    // sprintf(log, "handle_note_on_off note-on chan:%d note:%d vel:%d", channel, note, velocity);
    // Serial1.println((const char *)log);
#ifdef MONOPHONIC
    cv_1_handle_note_on_off(channel, note, velocity, is_note_on);
    cv_2_handle_note_on_off(channel, note, velocity, is_note_on);
    cv_3_handle_note_on_off(channel, note, velocity, is_note_on);
#endif
#ifdef DUOPHONIC
    duophonic_handle_note_on_off(channel, note, velocity, is_note_on);
    cv_3_handle_note_on_off(channel, note, velocity, is_note_on);
#endif
#ifdef TRIPHONIC
    triphonic_handle_note_on_off(channel, note, velocity, is_note_on);
#endif
    gates_handle_note_on_off(channel, note, velocity, is_note_on);
}

void handle_cc(uint8_t channel, uint8_t cc, uint8_t val)
{
    // char log[100];
    // sprintf(log, "cc chan:%d cc:%d val:%d", channel, cc, val);
    // Serial1.println((const char *)log);
    cv_1_handle_cc(channel, cc, val);
    cv_2_handle_cc(channel, cc, val);
    cv_3_handle_cc(channel, cc, val);
    gates_handle_cc(channel, cc, val);
}

void handle_clock()
{
    // char log[100];
    // sprintf(log, "clock");
    // Serial1.println((const char *)log);
    gates_handle_clock();
}

void handle_transport(uint8_t code)
{
    char log[100];
    sprintf(log, "transport %s",  code == MIDI_START ? "start" : code == MIDI_CONTINUE ? "continue" : "stop");
    Serial1.println((const char *)log);
    gates_handle_transport(code);
}

void handle_midi(uint8_t *msg)
{
    // blink
    digitalWrite(13, hi);
    hi = hi==1 ? 0 : 1;

    if (!msg) return;
    uint8_t channel = msg[0] & 0b00001111;
    uint8_t code = (msg[0] >> 4) & 0b00001111;

    // char log[10];
    // sprintf(log, "%d", code);
    // Serial1.println(log);

    switch(code){
        case MIDI_NOTE_ON:
        {
            uint8_t note = msg[1] & 0b01111111;
            uint8_t velocity = msg[2] & 0b01111111;

            // char log[30];
            // sprintf(log, "note-%d vel-%d", note, velocity);
            // Serial1.println(log);
            
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
        default: break;
    }
    gates_update();
}

void handle_realtime_midi(uint8_t *msg)
{
    // blink
    // digitalWrite(13, hi);
    // hi = hi==1 ? 0 : 1;

    if (!msg) return;
    uint8_t code = msg[0];

    // char log[10];
    // sprintf(log, "%d", code);
    // Serial1.println(log);

    switch(code){
        case MIDI_CLOCK:
        {
            handle_clock();
            break;
        }
        case MIDI_START:
        case MIDI_STOP:
        case MIDI_CONTINUE:
        {
            handle_transport(code);
            break;
        }
        default: break;
    }
    gates_update();
}

void midi_host_parse(uint8_t in)
{
    if ( midiHostParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        msg = midiHostParser.getMidiMsg();
        if(midiHostParser.getMidiMsgType() == midiXparser::realTimeMsgTypeMsk)
        {
            handle_realtime_midi(msg);
        }
        else
        {
            handle_midi(msg);
        }
    }
}

void midi_device_parse(uint8_t in)
{
    if ( midiDeviceParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        msg = midiDeviceParser.getMidiMsg();
        if(midiDeviceParser.getMidiMsgType() == midiXparser::realTimeMsgTypeMsk)
        {
            handle_realtime_midi(msg);
        }
        else
        {
            handle_midi(msg);
        }
    }
}

void midi_parser_init(void)
{
    midiHostParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk );
    midiDeviceParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk );
}