#ifndef POLYPHONY_H
#define POLYPHONY_H

void duophonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);
void triphonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on);

void push_note(struct stack_t *data, uint8_t note);
uint8_t pop_highest(struct stack_t *data);
uint8_t pop_lowest(struct stack_t *data);
uint8_t pop_last(struct stack_t *data);
uint8_t peek_last_duo(struct stack_t *data);
uint8_t peek_last_tri(struct stack_t *data);
void remove_note(struct stack_t *data, uint8_t note);


#endif