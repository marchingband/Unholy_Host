#ifndef GATES_H
#define GATES_H

void gates_init();
void gate_set(uint8_t pin, const uint8_t value);
uint8_t gate_get(uint8_t pin);
void trigger_start(uint8_t num_trig);
void clear_triggers(void);

#endif