#include <Arduino.h>
#include "handlers.h"
#include "config.h"
#include "constants.h"
#include "dac.h"
#include "gates.h"
#include "polyphony.h"

#define POLYPHONY_STACK_SIZE 20

struct stack_t {
    uint8_t *stack;
    uint8_t pointer;
};

uint8_t duo_voice_1_note = 0;
uint8_t duo_voice_2_note = 0;
bool duo_voice_1_free = true;
bool duo_voice_2_free = true;
uint8_t duo_stack_buf[POLYPHONY_STACK_SIZE] = {0};
struct stack_t duo_stack = {
    .stack = duo_stack_buf,
    .pointer = 0,
};

uint8_t tri_voice_1_note = 0;
uint8_t tri_voice_2_note = 0;
uint8_t tri_voice_3_note = 0;
bool tri_voice_1_free = true;
bool tri_voice_2_free = true;
bool tri_voice_3_free = true;
uint8_t tri_stack_buf[POLYPHONY_STACK_SIZE] = {0};
struct stack_t tri_stack = {
    .stack = tri_stack_buf,
    .pointer = 0,
};

void duophonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef DUOPHONIC
    if(is_note_on)
    {
        if(duo_voice_1_free)
        {
            push_note(&duo_stack, note);
            duo_voice_1_free = false;
            duo_voice_1_note = note;
            note_to_dac(0, note, CV_1_SCALE == V_OCT);
            gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
        }
        else if(duo_voice_2_free)
        {
            push_note(&duo_stack, note);
            duo_voice_2_free = false;
            duo_voice_2_note = note;
            note_to_dac(1, note, CV_1_SCALE == V_OCT);
            gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
        }
        else
        {
            push_note(&duo_stack, note);
            set_duo();
        }
    }
    else // it's note off
    {
        remove_note(&duo_stack, note);
        if(duo_stack.pointer > 1) // we need to replace it
        {
            set_duo();
        }
        else // we need to close a gate
        {
            if(duo_voice_1_note == note)
            {
                duo_voice_1_free = true;
                gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
            }
            else if(duo_voice_2_note == note)
            {
                duo_voice_2_free = true;
                gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
            }
        }
    }
#endif
}

void triphonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
#ifdef TRIPHONIC
    if(is_note_on)
    {
        if(tri_voice_1_free)
        {
            push_note(&tri_stack, note);
            tri_voice_1_free = false;
            tri_voice_1_note = note;
            note_to_dac(0, note, CV_1_SCALE == V_OCT);
            gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
        }
        else if(tri_voice_2_free)
        {
            push_note(&tri_stack, note);
            tri_voice_2_free = false;
            tri_voice_2_note = note;
            note_to_dac(1, note, CV_1_SCALE == V_OCT);
            gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
        }
        else if(tri_voice_3_free)
        {
            push_note(&tri_stack, note);
            tri_voice_3_free = false;
            tri_voice_3_note = note;
            note_to_dac(2, note, CV_1_SCALE == V_OCT);
            gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
        }
        else
        {
            push_note(&tri_stack, note);
            set_tri();
        }
    }
    else // it's note off
    {
        remove_note(&tri_stack, note);
        if(tri_stack.pointer > 2) // we need to replace it
        {
            set_tri();
        }
        else // we need to close a gate
        {
            if(tri_voice_1_note == note)
            {
                tri_voice_1_free = true;
                gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
            }
            else if(tri_voice_2_note == note)
            {
                tri_voice_2_free = true;
                gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
            }
            else if(tri_voice_3_note == note)
            {
                tri_voice_3_free = true;
                gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
            }
        }
    }
#endif
}

void set_tri(void)
{
#ifdef TRIPHONIC
    uint8_t a = CV_1_AND_2_AND_3_MODE == HIGHEST ? pop_highest(&tri_stack) : CV_1_AND_2_AND_3_MODE == LOWEST ? pop_lowest(&tri_stack) : pop_last(&tri_stack);
    uint8_t b = CV_1_AND_2_AND_3_MODE == HIGHEST ? pop_highest(&tri_stack) : CV_1_AND_2_AND_3_MODE == LOWEST ? pop_lowest(&tri_stack) : pop_last(&tri_stack);
    uint8_t c = CV_1_AND_2_AND_3_MODE == HIGHEST ? pop_highest(&tri_stack) : CV_1_AND_2_AND_3_MODE == LOWEST ? pop_lowest(&tri_stack) : pop_last(&tri_stack);
    push_note(&tri_stack, c);
    push_note(&tri_stack, b);
    push_note(&tri_stack, a);
    tri_voice_1_note = a;
    tri_voice_2_note = b;
    tri_voice_3_note = c;
    note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
    note_to_dac(1, tri_voice_2_note, CV_1_SCALE == V_OCT);
    note_to_dac(2, tri_voice_3_note, CV_1_SCALE == V_OCT);
#endif
}

void set_duo(void)
{
#ifdef DUOPHONIC
    uint8_t a = CV_1_AND_2_MODE == HIGHEST ? pop_highest(&duo_stack) : CV_1_AND_2_MODE == LOWEST ? pop_lowest(&duo_stack) : pop_last(&duo_stack);
    uint8_t b = CV_1_AND_2_MODE == HIGHEST ? pop_highest(&duo_stack) : CV_1_AND_2_MODE == LOWEST ? pop_lowest(&duo_stack) : pop_last(&duo_stack);
    push_note(&duo_stack, b);
    push_note(&duo_stack, a);
    duo_voice_1_note = a;
    duo_voice_2_note = b;
    note_to_dac(0, duo_voice_1_note, CV_1_SCALE == V_OCT);
    note_to_dac(1, duo_voice_2_note, CV_1_SCALE == V_OCT);
#endif
}

void push_note(struct stack_t *data, uint8_t note)
{
    if(data->pointer < POLYPHONY_STACK_SIZE)
    {
        data->stack[data->pointer] = note;
        data->pointer++;
    }
    else
    {
        for(int i=0; i<(POLYPHONY_STACK_SIZE - 1); i++)
        {
            data->stack[i] = data->stack[i+1];
        }
        data->stack[POLYPHONY_STACK_SIZE - 1] = note;
    }
}

uint8_t pop_highest(struct stack_t *data)
{
    uint8_t highest = 0;
    for(int i=0; i<data->pointer; i++)
    {
        if(data->stack[i] > highest)
        {
            highest = data->stack[i];
        }
    }
    remove_note(data, highest);
    return highest;
}

uint8_t pop_lowest(struct stack_t *data)
{
    uint8_t lowest = 127;
    for(int i=0; i<data->pointer; i++)
    {
        if(data->stack[i] < lowest)
        {
            lowest = data->stack[i];
        }
    }
    remove_note(data, lowest);
    return lowest;
}

uint8_t pop_last(struct stack_t *data)
{
    uint8_t note = data->stack[data->pointer - 1];
    data->pointer--;
    return note;
}

void remove_note(struct stack_t *data, uint8_t note)
{
    int i;
    for(i=0; i<data->pointer; i++)
    {
        if(data->stack[i] == note)
        {
            data->stack[i] = 0;
            data->pointer --;
            break;
        }
    }
    for(; i < data->pointer + 1; i++)
    {
        if(i < (POLYPHONY_STACK_SIZE - 2))
        {
            data->stack[i] = data->stack[i+1];
            data->stack[i+1] = 0;
        }
    }
}
