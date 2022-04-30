#ifndef HANDLERS_H
#define HANDLERS_H

void cv_1_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void cv_2_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void cv_3_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void duophonic_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void triphonic_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void gates_handle_note_on_off(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void gates_handle_duophonic_note_on_off(uint8_t channel, uint8_t voice, bool is_note_on);
void gates_handle_triphonic_note_on_off(uint8_t channel, uint8_t voice, bool is_note_on);
void cv_1_handle_cc(uint8_t channel, uint8_t cc, uint8_t val);
void cv_2_handle_cc(uint8_t channel, uint8_t cc, uint8_t val);
void cv_3_handle_cc(uint8_t channel, uint8_t cc, uint8_t val);
void gates_handle_cc(uint8_t channel, uint8_t cc, uint8_t val);
void gates_handle_clock(uint8_t channel);
void gates_handle_transport(uint8_t channel, uint8_t code);
void init_handlers(void);

#endif