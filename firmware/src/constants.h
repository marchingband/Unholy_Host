#ifndef CONSTANTS_H
#define CONSTANTS_H

// POLY MODES
#define MONOPHONIC_MODE 0
#define DUOPHONIC_MODE 1
#define TRIPHONIC_MODE 2

// CV SCALES
#define V_OCT 0
#define V_OCT_MIN 33
#define V_OCT_MAX 93
#define HZ_V 1
#define HZ_V_MIN 9
#define HZ_V_MAX 72
#define NUM_DACS 3

#define NUM_NOTES_HZ_V (HZ_V_MAX - HZ_V_MIN)    // 63 // A0 to G#4
#define NUM_NOTES_V_OCT (V_OCT_MAX - V_OCT_MIN) // 60 // A0 to G#4

// CV MODES
#define HIGHEST 0
#define LOWEST 1
#define LAST 2

// CV SOURCES
#define NOTE 0
#define VELOCITY 1
#define CC_1 2
#define CC_2 3
#define CC_3 4
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
#define MONOPHONIC_ON_OFF 13
#define DUOPHONIC_ON_OFF_VOICE_1 14
#define DUOPHONIC_ON_OFF_VOICE_2 15
#define TRIPHONIC_ON_OFF_VOICE_1 16
#define TRIPHONIC_ON_OFF_VOICE_2 17
#define TRIPHONIC_ON_OFF_VOICE_3 18
#define CC1_HI_LOW 19
#define CC2_HI_LOW 20
#define CC3_HI_LOW 21

// MIDI CODES
#define MIDI_NOTE_OFF 8
#define MIDI_NOTE_ON 9
#define MIDI_CC 11
#define MIDI_PITCH_BEND 14
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
#define C_N2 0
#define Cs_N2 1
#define D_N2 2
#define Ds_N2 3
#define E_N2 4
#define F_N2 5
#define Fs_N2 6
#define G_N2 7
#define Gs_N2 8
#define A_N2 9
#define As_N2 10
#define B_N2 11
#define C_N1 12
#define Cs_N1 13
#define D_N1 14
#define Ds_N1 15
#define E_N1 16
#define F_N1 17
#define Fs_N1 18
#define G_N1 19
#define Gs_N1 20
#define A_N1 21
#define As_N1 22
#define B_N1 23
#define C_0 24
#define Cs_0 25
#define D_0 26
#define Ds_0 27
#define E_0 28
#define F_0 29
#define Fs_0 30
#define G_0 31
#define Gs_0 32
#define A_0 33
#define As_0 34
#define B_0 35
#define C_1 36
#define Cs_1 37
#define D_1 38
#define Ds_1 39
#define E_1 40
#define F_1 41
#define Fs_1 42
#define G_1 43
#define Gs_1 44
#define A_1 45
#define As_1 46
#define B_1 47
#define C_2 48
#define Cs_2 49
#define D_2 50
#define Ds_2 51
#define E_2 52
#define F_2 53
#define Fs_2 54
#define G_2 55
#define Gs_2 56
#define A_2 57
#define As_2 58
#define B_2 59
#define C_3 60
#define Cs_3 61
#define D_3 62
#define Ds_3 63
#define E_3 64
#define F_3 65
#define Fs_3 66
#define G_3 67
#define Gs_3 68
#define A_3 69
#define As_3 70
#define B_3 71
#define C_4 72
#define Cs_4 73
#define D_4 74
#define Ds_4 75
#define E_4 76
#define F_4 77
#define Fs_4 78
#define G_4 79
#define Gs_4 80
#define A_4 81
#define As_4 82
#define B_4 83
#define C_5 84
#define Cs_5 85
#define D_5 86
#define Ds_5 87
#define E_5 88
#define F_5 89
#define Fs_5 90
#define G_5 91
#define Gs_5 92
#define A_5 93
#define As_5 94
#define B_5 95
#define C_6 96
#define Cs_6 97
#define D_6 98
#define Ds_6 99
#define E_6 100
#define F_6 101
#define Fs_6 102
#define G_6 103
#define Gs_6 104
#define A_6 105
#define As_6 106
#define B_6 107
#define C_7 108
#define Cs_7 109
#define D_7 110
#define Ds_7 111
#define E_7 112
#define F_7 113
#define Fs_7 114
#define G_7 115
#define Gs_7 116
#define A_7 117
#define As_7 118
#define B_7 119
#define C_8 120
#define Cs_8 121
#define D_8 122
#define Ds_8 123
#define E_8 124
#define F_8 125
#define Fs_8 126
#define G_8 127

#endif