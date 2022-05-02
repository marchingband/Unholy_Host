#ifndef CONSTANTS_H
#define CONSTANTS_H

// CV SCALES
#define V_OCT 0
#define V_OCT_MIN 33
#define V_OCT_MAX 93
#define HZ_V 1
#define HZ_V_MIN 9
#define HZ_V_MAX 72
#define NUM_DACS 3

// CV MODES
#define HIGHEST 0
#define LOWEST 1
#define LAST 2

// CV SOURCES
#define NOTE 0
#define VELOCITY 1
#define CC1 2
#define CC2 3
#define CC3 4
#define PITCHBEND 5

// GATE SOURCES
#define NOTE_ON_OFF 0
#define CLOCK_1 1
#define CLOCK_2 2
#define CLOCK_3 3
#define CLOCK_4 4
#define CLOCK_5 5
#define CLOCK_6 6
#define CLOCK_7 7
#define CLOCK_8 8
#define RESET_1 9
#define RESET_2 10
#define RESET_3 11
#define TRANSPORT 12
#define DUOPHONIC_ON_OFF_VOICE_1 13
#define DUOPHONIC_ON_OFF_VOICE_2 14
#define TRIPHONIC_ON_OFF_VOICE_1 15
#define TRIPHONIC_ON_OFF_VOICE_2 16
#define TRIPHONIC_ON_OFF_VOICE_3 17
#define CC1_HI_LOW 18
#define CC2_HI_LOW 19
#define CC3_HI_LOW 20

// MIDI CODES
#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_CC 11
#define MIDI_CLOCK 248
#define MIDI_START 250
#define MIDI_CONTINUE 251
#define MIDI_STOP 252
#define MIDI_PROGRAM_CHANGE 12

#define MIDI_CC_VOLUME 7 // channel volume
#define MIDI_CC_PAN 10
#define MIDI_CC_EXP 11
#define MIDI_CC_SUSTAIN 64 // pedal
#define MIDI_CC_RELEASE 72 // release time
#define MIDI_CC_MUTE 120 // instant mute
#define MIDI_CC_RESET 121 // reset all controllers

// MIDI NOTES
#define C_2 0
#define Cs_2 1
#define D_2 2
#define Ds_2 3
#define E_2 4
#define F_2 5
#define Fs_2 6
#define G_2 7
#define Gs_2 8
#define A_2 9
#define As_2 10
#define B_2 11
#define C_1 12
#define Cs_1 13
#define D_1 14
#define Ds_1 15
#define E_1 16
#define F_1 17
#define Fs_1 18
#define G_1 19
#define Gs_1 20
#define A_1 21
#define As_1 22
#define B_1 23
#define C0 24
#define Cs0 25
#define D0 26
#define Ds0 27
#define E0 28
#define F0 29
#define Fs0 30
#define G0 31
#define Gs0 32
#define A0 33
#define As0 34
#define B0 35
#define C1 36
#define Cs1 37
#define D1 38
#define Ds1 39
#define E1 40
#define F1 41
#define Fs1 42
#define G1 43
#define Gs1 44
#define A1 45
#define As1 46
#define B1 47
#define C2 48
#define Cs2 49
#define D2 50
#define Ds2 51
#define E2 52
#define F2 53
#define Fs2 54
#define G2 55
#define Gs2 56
#define A2 57
#define As2 58
#define B2 59
#define C3 60
#define Cs3 61
#define D3 62
#define Ds3 63
#define E3 64
#define F3 65
#define Fs3 66
#define G3 67
#define Gs3 68
#define A3 69
#define As3 70
#define B3 71
#define C4 72
#define Cs4 73
#define D4 74
#define Ds4 75
#define E4 76
#define F4 77
#define Fs4 78
#define G4 79
#define Gs4 80
#define A4 81
#define As4 82
#define B4 83
#define C5 84
#define Cs5 85
#define D5 86
#define Ds5 87
#define E5 88
#define F5 89
#define Fs5 90
#define G5 91
#define Gs5 92
#define A5 93
#define As5 94
#define B5 95
#define C6 96
#define Cs6 97
#define D6 98
#define Ds6 99
#define E6 100
#define F6 101
#define Fs6 102
#define G6 103
#define Gs6 104
#define A6 105
#define As6 106
#define B6 107
#define C7 108
#define Cs7 109
#define D7 110
#define Ds7 111
#define E7 112
#define F7 113
#define Fs7 114
#define G7 115
#define Gs7 116
#define A7 117
#define As7 118
#define B7 119
#define C8 120
#define Cs8 121
#define D8 122
#define Ds8 123
#define E8 124
#define F8 125
#define Fs8 126
#define G8 127

#endif