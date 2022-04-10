#include <Arduino.h>
#include "midiXparser.h"
#include "midi.h"

#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_CC 11
#define MIDI_PROGRAM_CHANGE 12

#define MIDI_CC_VOLUME 7 // channel volume
#define MIDI_CC_PAN 10
#define MIDI_CC_EXP 11
#define MIDI_CC_SUSTAIN 64 // pedal
#define MIDI_CC_RELEASE 72 // release time
#define MIDI_CC_MUTE 120 // instant mute
#define MIDI_CC_RESET 121 // reset all controllers


midiXparser midiHostParser;
midiXparser midiDeviceParser;

int hi = 0;

void handle_note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{
    Serial1.println("note-on");
    // Serial1.println("note-on ch:%d note:%d vel:%d", channel, note, velocity);
}

void handle_note_off(uint8_t channel, uint8_t note)
{
    Serial1.println("note-off");
    // Serial1.println("note-off ch:%d note:%d", channel, note);
}

void handle_cc_exp(uint8_t channel, uint8_t val)
{
    Serial1.println("expression");
    // Serial1.println("expression ch:%d val:%d", channel, val);
}

void handle_midi(uint8_t *msg)
{
    // blink
    digitalWrite(13, hi);
    hi = hi==1 ? 0 : 1;
    // blink

    if (!msg) return;
    uint8_t channel = msg[0] & 0b00001111;
    uint8_t code = (msg[0] >> 4) & 0b00001111;
    switch(code){
        case MIDI_NOTE_ON:
        {
            uint8_t note = msg[1] & 0b01111111;
            uint8_t velocity = msg[2] & 0b01111111;
            handle_note_on(channel, note, velocity);
            break;
        }
        case MIDI_NOTE_OFF:
        {
            uint8_t note = msg[1] & 0b01111111;
            uint8_t velocity = msg[2] & 0b01111111;
            handle_note_off(channel, note);
            break;
        }
        case MIDI_PROGRAM_CHANGE:
        {
            break;
        }
        case MIDI_CC:
        {
            uint8_t CC = msg[1] & 0b01111111;
            uint8_t val = msg[2]  & 0b01111111;
            switch (CC){
                case MIDI_CC_EXP:
                {
                    handle_cc_exp(channel, val);
                }
                case MIDI_CC_VOLUME:
                case MIDI_CC_PAN:
                case MIDI_CC_SUSTAIN:
                case MIDI_CC_RELEASE:
                case MIDI_CC_MUTE:
                case MIDI_CC_RESET:
                {
                    break;
                }
            }
            break;
        }
    }

}

void midi_host_parse(uint8_t in)
{
    if ( midiHostParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        if ( 
                midiHostParser.isMidiStatus(midiXparser::noteOnStatus) || 
                midiHostParser.isMidiStatus(midiXparser::noteOffStatus) || 
                midiHostParser.isMidiStatus(midiXparser::programChangeStatus) || 
                midiHostParser.isMidiStatus(midiXparser::controlChangeStatus) 
            ) 
        {
            uint8_t *msg = midiHostParser.getMidiMsg();
            handle_midi(msg);
        }
    }
}

void midi_device_parse(uint8_t in)
{
    if ( midiDeviceParser.parse( in ) )  // Do we received a channel voice msg ?
    {
        if ( 
                midiDeviceParser.isMidiStatus(midiXparser::noteOnStatus) || 
                midiDeviceParser.isMidiStatus(midiXparser::noteOffStatus) || 
                midiDeviceParser.isMidiStatus(midiXparser::programChangeStatus) || 
                midiDeviceParser.isMidiStatus(midiXparser::controlChangeStatus) 
            ) 
        {
            uint8_t *msg = midiDeviceParser.getMidiMsg();
            handle_midi(msg);
        }
    }
}

void midi_parser_init(void)
{
    midiHostParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );
    midiDeviceParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk );
}