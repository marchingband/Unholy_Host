
#ifndef CONFIG_H
#define CONFIG_H

#include "constants.h"
#include <stdint.h>
#include <stdbool.h>
// #include "Arduino.h"

struct config_t {
    bool valid;
    bool calibration_mode;
    uint8_t POLYPHONY_MODE;
    uint8_t CV_1_SOURCE;
    uint8_t CV_2_SOURCE;
    uint8_t CV_3_SOURCE;
    uint8_t CV_1_AND_2_MODE;
    uint8_t CV_1_AND_2_AND_3_MODE;
    uint8_t CV_1_SCALE;
    uint8_t CV_2_SCALE;
    uint8_t CV_3_SCALE;
    uint8_t PITCH_BEND_RANGE;
    uint8_t CC1_COMMAND;
    uint8_t CC2_COMMAND;
    uint8_t CC3_COMMAND;
    uint8_t CLOCK_1_DIVIDER;
    uint8_t CLOCK_2_DIVIDER;
    uint8_t CLOCK_3_DIVIDER;
    uint8_t CLOCK_4_DIVIDER;
    uint8_t CLOCK_5_DIVIDER;
    uint8_t CLOCK_6_DIVIDER;
    uint8_t CLOCK_7_DIVIDER;
    uint8_t CLOCK_8_DIVIDER;
    uint8_t RESET_1_BEATS;
    uint8_t RESET_2_BEATS;
    uint8_t RESET_3_BEATS;
    uint8_t GATE_1_SOURCE;
    uint8_t GATE_1_NOTE;
    uint8_t GATE_1_INVERT;
    uint8_t GATE_2_SOURCE;
    uint8_t GATE_2_NOTE;
    uint8_t GATE_2_INVERT;
    uint8_t GATE_3_SOURCE;
    uint8_t GATE_3_NOTE;
    uint8_t GATE_3_INVERT;
    uint8_t GATE_4_SOURCE;
    uint8_t GATE_4_NOTE;
    uint8_t GATE_4_INVERT;
    uint8_t GATE_5_SOURCE;
    uint8_t GATE_5_NOTE;
    uint8_t GATE_5_INVERT;
    uint8_t GATE_6_SOURCE;
    uint8_t GATE_6_NOTE;
    uint8_t GATE_6_INVERT;
    uint8_t GATE_7_SOURCE;
    uint8_t GATE_7_NOTE;
    uint8_t GATE_7_INVERT;
    uint8_t GATE_8_SOURCE;
    uint8_t GATE_8_NOTE;
    uint8_t GATE_8_INVERT;
    double CAL_1_0;
    double CAL_1_1;
    double CAL_1_2;
    double CAL_1_3;
    double CAL_1_4;
    double CAL_1_5;
    double CAL_1_6;
    double CAL_2_0;
    double CAL_2_1;
    double CAL_2_2;
    double CAL_2_3;
    double CAL_2_4;
    double CAL_2_5;
    double CAL_2_6;
    double CAL_3_0;
    double CAL_3_1;
    double CAL_3_2;
    double CAL_3_3;
    double CAL_3_4;
    double CAL_3_5;
    double CAL_3_6;
};

// #define CALIBRATION_MODE // uncomment this line to enter calibration mode

// #define MONOPHONIC // polyphony mode: MONOPHONIC || DUOPHONIC || TRIPHONIC 

// #if defined(MONOPHONIC)
//     #define CV_1_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3
//     #define CV_2_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3
//     #define CV_3_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3
// #elif defined(DUOPHONIC)
//     #define CV_1_AND_2_MODE        LAST // HIGHEST || LOWEST || LAST 
//     #define CV_3_SOURCE            NOTE // NOTE || VELOCITY || CC1 || CC2 || CC3 ||
// #elif defined(TRIPHONIC)
//     #define CV_1_AND_2_AND_3_MODE  LAST // HIGHEST || LOWEST || LAST
// #endif

// #define CV_1_SCALE V_OCT  // V_OCT || HZ_V  (CV_1_SCALE determines scale for all polyphonic cvs)
// #define CV_2_SCALE V_OCT  // V_OCT || HZ_V
// #define CV_3_SCALE V_OCT  // V_OCT || HZ_V

// #define PITCH_BEND_RANGE 12 // 0 to infinity (in semitones)

// // CCs will output CV scaled from the sent CC value, listening on n number CCx
// #define CC1_COMMAND      7    // 0...119
// #define CC2_COMMAND      10   // 0...119
// #define CC3_COMMAND      11   // 0...119

// // clocks will send trigger every n number of midi clocks (there are 24 midi clocks per beat)
// #define CLOCK_1_DIVIDER  3     // 1...infinity
// #define CLOCK_2_DIVIDER  6     // 1...infinity
// #define CLOCK_3_DIVIDER  12    // 1...infinity
// #define CLOCK_4_DIVIDER  24    // 1...infinity
// #define CLOCK_5_DIVIDER  48    // 1...infinity
// #define CLOCK_6_DIVIDER  96    // 1...infinity
// #define CLOCK_7_DIVIDER  192   // 1...infinity
// #define CLOCK_8_DIVIDER  384   // 1...infinity

// // resets will send trigger on MIDI_START, MIDI_STOP, MIDI CONTINUE, and every n number of beats (where 24 midi clocks is 1 beat)
// #define RESET_1_BEATS    4    // 0(just at the start)...infinity
// #define RESET_2_BEATS    16   // 0(just at the start)...infinity
// #define RESET_3_BEATS    32   // 0(just at the start)...infinity

// // TRANSPORT sets gate high after MIDI_START or MIDI_CONTINUE, and low after MIDI_STOP
// // NOTE_ON_OFF sets gate high after NOTE_ON and low after NOTE_OFF, listening on n note number
// // CC1, CC2, CC3 will set gate high if value received on CCx (number defined above for each) is greater then 63, and low if less then 64
// #define GATE_1_SOURCE    MONOPHONIC_ON_OFF // NOTE_ON_OFF || MONOPHONIC_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_1_NOTE      C_6          // C_2...G8 || 0...127
// // #define GATE_1_INVERT
// #define GATE_2_SOURCE    CLOCK_2 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_2_NOTE      Cs_6         // C_2...G8 || 0...127
// // #define GATE_2_INVERT
// #define GATE_3_SOURCE    CLOCK_3 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_3_NOTE      D_6          // C_2...G8 || 0...127
// // #define GATE_3_INVERT
// #define GATE_4_SOURCE    CLOCK_4 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_4_NOTE      Ds_6         // C_2...G8 || 0...127
// // #define GATE_4_INVERT
// #define GATE_5_SOURCE    CLOCK_5 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_5_NOTE      E_6          // C_2...G8 || 0...127
// // #define GATE_5_INVERT
// #define GATE_6_SOURCE    CLOCK_6 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_6_NOTE      F_6          // C_2...G8 || 0...127
// // #define GATE_6_INVERT
// #define GATE_7_SOURCE    CLOCK_7 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_7_NOTE      Fs_6         // C_2...G8 || 0...127
// // #define GATE_7_INVERT
// #define GATE_8_SOURCE    CLOCK_8 // NOTE_ON_OFF || DUOPHONIC_ON_OFF_VOICE_1 || DUOPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_1 || TRIPHONIC_ON_OFF_VOICE_2 || TRIPHONIC_ON_OFF_VOICE_3 || CC1_HI_LOW || CC2_HI_LOW || CC3_HI_LOW || RESET_1 || RESET_2 || RESET_3 || TRANSPORT || CLOCK_1 || CLOCK_2 || CLOCK_3 || CLOCK_4 || CLOCK_5 || CLOCK_6 || CLOCK_7 || CLOCK_8
// #define GATE_8_NOTE      G_6          // C_2...G8 || 0...127
// // #define GATE_8_INVERT

void init_config( void );
void encode_config(uint8_t *dest);
void decode_config(uint8_t *src);
void encode_float(uint8_t *dest, double in);
double decode_float(uint8_t *src);
void save_config( void );

#endif