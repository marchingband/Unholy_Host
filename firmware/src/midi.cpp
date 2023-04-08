#include <Arduino.h>
#include "midiXparser.h"
#include "midi.h"
#include "dac.h"
#include "gates.h"
#include "handlers.h"
#include "config.h"
#include "constants.h"
#include "usb_device.h"

#define DAC_1 0
#define DAC_2 1
#define DAC_3 2

midiXparser midiHostParser;
midiXparser midiDeviceParser;

uint8_t sysex_buf[500];
int sysex_p = 0;

int hi = 0;

uint8_t *msg;
uint16_t pitch_bend_state = 8192;

extern struct config_t *config;

void handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    // char log[100];
    // sprintf(log, "handle_note_on_off note-on chan:%d note:%d vel:%d", channel, note, velocity);
    // Serial1.println((const char *)log);
// #ifdef MONOPHONIC
    if(config->POLYPHONY_MODE == MONOPHONIC_MODE)
    {
        cv_1_handle_note_on_off(channel, note, velocity, is_note_on);
        cv_2_handle_note_on_off(channel, note, velocity, is_note_on);
        cv_3_handle_note_on_off(channel, note, velocity, is_note_on);
        gates_handle_monophonic_note_on_off(channel, note, is_note_on);
    }
// #endif
// #ifdef DUOPHONIC
    else if(config->POLYPHONY_MODE == DUOPHONIC_MODE)
    {
        duophonic_handle_note_on_off(channel, note, velocity, is_note_on);
        cv_3_handle_note_on_off(channel, note, velocity, is_note_on);
    }
// #endif
// #ifdef TRIPHONIC
    else if(config->POLYPHONY_MODE == TRIPHONIC_MODE)
    {
        triphonic_handle_note_on_off(channel, note, velocity, is_note_on);
    }
// #endif
    gates_handle_note_on_off(channel, note, velocity, is_note_on);
    gates_update();
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
    gates_update();
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
    // char log[100];
    // sprintf(log, "transport %s",  code == MIDI_START ? "start" : code == MIDI_CONTINUE ? "continue" : "stop");
    // Serial1.println((const char *)log);
    gates_handle_transport(code);
}

void handle_pitch_bend(uint8_t channel, uint16_t pitch_bend)
{
    // pitch_bend = 0 to 16,383 // 8192 is none
    // Serial1.print("bend ");
    // Serial1.println(pitch_bend);
    pitch_bend_state = pitch_bend;
    dacs_handle_pitch_bend();
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
        case MIDI_PITCH_BEND:
        {
            uint8_t lsb = msg[1] & 0b01111111;
            uint8_t msb = msg[2]  & 0b01111111;
            uint16_t pitch_bend = ((msb << 7) | lsb);
            handle_pitch_bend(channel + 1, pitch_bend);
            break;
        }
        default: break;
    }
    // gates_update();
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

void handle_sysex(){
    uint len = midiDeviceParser.getSysExMsgLen();
    
    Serial1.print("GOT SYSEX len ");
    Serial1.println(len);

    if( (sysex_buf[1] != MANUFACTURER_ID_1) ||
        (sysex_buf[2] != MANUFACTURER_ID_2) ||
        (sysex_buf[3] != MANUFACTURER_ID_3))
    {
        Serial1.println("MANUFACTURER_ID incorrect");
        return; // not for us
    }
    else
    {
        Serial1.println("MANUFACTURER_ID matches");
    }

    if(sysex_buf[4] == SYSEX_TYPE_REQUEST_CONFIG)
    {
        Serial1.println("got SYSEX_TYPE_REQUEST_CONFIG");
        usb_device_send_config();
    } 
    else if(sysex_buf[4] == SYSEX_TYPE_CONFIG)
    {
        Serial1.println("got SYSEX_TYPE_CONFIG");
        decode_config(&sysex_buf[5]);
        usb_device_send_ack();
    } 
    else if(sysex_buf[4] == SYSEX_TYPE_CAL_MODE_ON)
    {
        Serial1.println("got SYSEX_TYPE_CAL_MODE_ON");
        config->calibration_mode = 1;
        usb_device_send_ack();
    } 
    else if(sysex_buf[4] == SYSEX_TYPE_CAL_MODE_OFF)
    {
        Serial1.println("got SYSEX_TYPE_CAL_MODE_OFF");
        config->calibration_mode = 0;
        usb_device_send_ack();
    } 
    else 
    {
        Serial1.println("got unknown SYSEX TYPE");
    }
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

uint8_t is_sysex = 0;

void midi_device_parse(uint8_t in)
{
    Serial1.println("in");
    Serial1.println(in);
    uint8_t is_msg = midiDeviceParser.parse( in );
    if ( is_msg )  // Do we received a channel voice msg ?
    {
        msg = midiDeviceParser.getMidiMsg();
        if(midiDeviceParser.getMidiMsgType() == midiXparser::realTimeMsgTypeMsk)
        {
            handle_realtime_midi(msg);
        }
        else if(midiDeviceParser.getMidiMsgType() == midiXparser::channelVoiceMsgTypeMsk)
        {
            handle_midi(msg);
        }
        else if(midiDeviceParser.getMidiMsgType() == midiXparser::sysExMsgTypeMsk)
        {
            handle_sysex();
            sysex_p = 0;
            for(int i=0; i<500; i++){
                sysex_buf[i] = 0;
            }
        }
    } 
    if(midiDeviceParser.isSysExMode() && midiDeviceParser.isByteCaptured())
    {
        uint8_t byte = midiDeviceParser.getByte();
        sysex_buf[sysex_p++] = byte;
        // Serial1.println("byte");
        // Serial1.println(byte);
    }

}

void midi_parser_init(void)
{
    midiHostParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk );
    midiDeviceParser.setMidiMsgFilter( midiXparser::channelVoiceMsgTypeMsk | midiXparser::realTimeMsgTypeMsk | midiXparser::sysExMsgTypeMsk );
}