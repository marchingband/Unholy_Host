#include <Arduino.h>
#include "handlers.h"
#include "config.h"
#include "constants.h"
#include "dac.h"
#include "gates.h"
#include "polyphony.h"

struct stack_t {
    uint8_t *stack;
    uint8_t pointer;
};

uint8_t duo_voice_1_note = 0;
uint8_t duo_voice_2_note = 0;
uint8_t duo_stack[10] = {0};
bool duo_voice_1_free = true;
bool duo_voice_2_free = true;
struct stack_t duo_stack = {
    .stack = duo_stack,
    .pointer = 0,
};

uint8_t tri_voice_1_note = 0;
uint8_t tri_voice_2_note = 0;
uint8_t tri_voice_3_note = 0;
uint8_t tri_stack[10] = {0};
bool tri_voice_1_free = true;
bool tri_voice_2_free = true;
bool tri_voice_3_free = true;
struct stack_t tri_stack = {
    .stack = tri_stack,
    .pointer = 0,
};

void duophonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    switch(CV_1_AND_2_MODE){
        case HIGHEST:
        {
            if(is_note_on)
            {
                if(duo_voice_1_free)
                {
                    duo_voice_1_free = false;
                    duo_voice_1_note = note;
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                }
                else if(duo_voice_2_free)
                {
                    duo_voice_2_free = false;
                    duo_voice_2_note = note;
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                }
                else if((note > duo_voice_1_note) && (note > duo_voice_2_note)) // new is highest
                {
                    if(duo_voice_1_note > duo_voice_2_note)
                    {
                        push_note(&duo_stack, duo_voice_2_note);
                        duo_voice_2_note = note;
                        note_to_dac(1, note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        push_note(&duo_stack, duo_voice_1_note);
                        duo_voice_1_note = note;
                        note_to_dac(0, note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(note > duo_voice_1_note)
                {
                    push_note(&duo_stack, duo_voice_1_note);
                    duo_voice_1_note = note;
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                }
                else if(note > duo_voice_2_note)
                {
                    push_note(&duo_stack, duo_voice_2_note);
                    duo_voice_2_note = note;
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                }
                else // its the lowest
                {
                    push_note(&duo_stack, note);
                }       
            }
            else // it's note off
            {
                if(duo_voice_1_note == note)
                {
                    if(duo_stack.pointer > 0)
                    {
                        duo_voice_1_note = pop_highest(&duo_stack);
                        note_to_dac(0, duo_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        duo_voice_1_free = true;
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(duo_voice_2_note == note)
                {
                    if(duo_stack.pointer > 0)
                    {
                        duo_voice_2_note = pop_highest(&duo_stack);
                        note_to_dac(1, duo_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        duo_voice_2_free = true;
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else
                {
                    remove_note(&duo_stack, note);
                }
            }
            break;
        }
        case LOWEST:
        {
            if(is_note_on)
            {
                if(duo_voice_1_free)
                {
                    duo_voice_1_free = false;
                    duo_voice_1_note = note;
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                }
                else if(duo_voice_2_free)
                {
                    duo_voice_2_free = false;
                    duo_voice_2_note = note;
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                }
                else if((note < duo_voice_1_note) && (note < duo_voice_2_note)) // new is lowest
                {
                    if(duo_voice_1_note < duo_voice_2_note)
                    {
                        push_note(&duo_stack, duo_voice_2_note);
                        duo_voice_2_note = note;
                        note_to_dac(1, note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        push_note(&duo_stack, duo_voice_1_note);
                        duo_voice_1_note = note;
                        note_to_dac(0, note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(note < duo_voice_1_note)
                {
                    push_note(&duo_stack, duo_voice_1_note);
                    duo_voice_1_note = note;
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                }
                else if(note < duo_voice_2_note)
                {
                    push_note(&duo_stack, duo_voice_2_note);
                    duo_voice_2_note = note;
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                }
                else // its the lowest
                {
                    push_note(&duo_stack, note);
                }       
            }
            else // it's note off
            {
                if(duo_voice_1_note == note)
                {
                    if(duo_stack.pointer > 0)
                    {
                        duo_voice_1_note = pop_lowest(&duo_stack);
                        note_to_dac(0, duo_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        duo_voice_1_free = true;
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(duo_voice_2_note == note)
                {
                    if(duo_stack.pointer > 0)
                    {
                        duo_voice_2_note = pop_lowest(&duo_stack);
                        note_to_dac(1, duo_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        duo_voice_2_free = true;
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else
                {
                    remove_note(&duo_stack, note);
                }
            }
            break;
        }
        case LAST:
        {
            if(is_note_on)
            {
                if(duo_voice_1_free)
                {
                    duo_voice_1_free = false;
                    duo_voice_1_note = note;
                    push_note(&duo_stack, note);
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                }
                else if(duo_voice_2_free)
                {
                    duo_voice_2_free = false;
                    duo_voice_2_note = note;
                    push_note(&duo_stack, note);
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                }
                else
                {
                    uint8_t last_note = peek_last(&duo_stack);
                    if(last_note == duo_voice_1_note)
                    {
                        duo_voice_1_note = note;
                        push_note(&duo_stack, note);
                        note_to_dac(0, note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                    else if(last_note == duo_voice_2_note)
                    {
                        duo_voice_2_note = note;
                        push_note(&duo_stack, note);
                        note_to_dac(1, note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                }
            }
            else // it's note off
            {
                if(duo_voice_1_note == note)
                {
                    remove_note(%duo_stack, note);
                    if(duo_stack.pointer > 1)
                    {
                        duo_voice_1_note = peek_last(&duo_stack);
                        note_to_dac(0, duo_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        duo_voice_1_free = true;
                        gates_handle_duophonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(duo_voice_2_note == note)
                {
                    if(duo_stack.pointer > 1)
                    {
                        duo_voice_2_note = peek_last(&duo_stack);
                        note_to_dac(1, duo_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        duo_voice_2_free = true;
                        gates_handle_duophonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else
                {
                    remove_note(&duo_stack, note);
                }
            }
            break;
        }
    }
}

void triphonic_process_event(uint8_t channel, uint8_t note, uint8_t velocity, bool is_note_on)
{
    switch(CV_1_AND_2_AND_3_MODE){
        case HIGHEST:
        {
            if(is_note_on)
            {
                if(tri_voice_1_free)
                {
                    tri_voice_1_free = false;
                    tri_voice_1_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                }
                else if(tri_voice_2_free)
                {
                    tri_voice_2_free = false;
                    tri_voice_2_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                }
                else if(tri_voice_3_free)
                {
                    tri_voice_3_free = false;
                    tri_voice_3_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(2, note, CV_3_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                }
                else
                {
                    push_note(&tri_stack, note);

                    tri_voice_1_note = pop_highest(&tri_stack);
                    note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 1, is_note_on);

                    tri_voice_2_note = pop_highest(&tri_stack);
                    note_to_dac(1, tri_voice_2_note, CV_2_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 2, is_note_on);

                    tri_voice_3_note = pop_highest(&tri_stack);
                    note_to_dac(2, tri_voice_3_note, CV_3_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                }
            }
            else // it's note off
            {
                if(tri_voice_1_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_1_note = pop_highest(&tri_stack);
                        note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        tri_voice_1_free = true;
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(tri_voice_2_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_2_note = pop_highest(&tri_stack);
                        note_to_dac(1, tri_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        tri_voice_2_free = true;
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else if(tri_voice_3_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_3_note = pop_highest(&tri_stack);
                        note_to_dac(2, tri_voice_3_note, CV_3_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                    else
                    {
                        tri_voice_3_free = true;
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                }
                else
                {
                    remove_note(&tri_stack, note);
                }
            }
            break;
        }
        case LOWEST:
        {
            if(is_note_on)
            {
                if(tri_voice_1_free)
                {
                    tri_voice_1_free = false;
                    tri_voice_1_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                }
                else if(tri_voice_2_free)
                {
                    tri_voice_2_free = false;
                    tri_voice_2_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                }
                else if(tri_voice_3_free)
                {
                    tri_voice_3_free = false;
                    tri_voice_3_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(2, note, CV_3_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                }
                else
                {
                    push_note(&tri_stack, note);
                    tri_voice_1_note = pop_lowest(&tri_stack);
                    note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    tri_voice_2_note = pop_lowest(&tri_stack);
                    note_to_dac(1, tri_voice_2_note, CV_2_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    tri_voice_3_note = pop_lowest(&tri_stack);
                    note_to_dac(2, tri_voice_3_note, CV_3_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                }
            }
            else // it's note off
            {
                if(tri_voice_1_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_1_note = pop_lowest(&tri_stack);
                        note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        tri_voice_1_free = true;
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(tri_voice_2_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_2_note = pop_lowest(&tri_stack);
                        note_to_dac(1, tri_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        tri_voice_2_free = true;
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else if(tri_voice_3_note == note)
                {
                    if(tri_stack.pointer > 0)
                    {
                        tri_voice_3_note = pop_lowest(&tri_stack);
                        note_to_dac(2, tri_voice_3_note, CV_3_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                    else
                    {
                        tri_voice_3_free = true;
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                }
                else
                {
                    remove_note(&tri_stack, note);
                }
            }
            break;
        }
        case LAST:
        {
            if(is_note_on)
            {
                if(tri_voice_1_free)
                {
                    tri_voice_1_free = false;
                    tri_voice_1_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(0, note, CV_1_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                }
                else if(tri_voice_2_free)
                {
                    tri_voice_2_free = false;
                    tri_voice_2_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(1, note, CV_2_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                }
                else if(tri_voice_3_free)
                {
                    tri_voice_3_free = false;
                    tri_voice_3_note = note;
                    push_note(&tri_stack, note);
                    note_to_dac(2, note, CV_3_SCALE == V_OCT);
                    gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                }
                else
                {
                    uint8_t last_note = peek_last(&tri_stack);
                    if(last_note == tri_voice_1_note)
                    {
                        tri_voice_1_note = note;
                        push_note(&tri_stack, note);
                        note_to_dac(0, note, CV_1_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                    else if(last_note == tri_voice_2_note)
                    {
                        tri_voice_2_note = note;
                        push_note(&tri_stack, note);
                        note_to_dac(1, note, CV_2_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                    else if(last_note == tri_voice_3_note)
                    {
                        tri_voice_3_note = note;
                        push_note(&tri_stack, note);
                        note_to_dac(2, note, CV_3_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                }
            }
            else // it's note off
            {
                if(tri_voice_1_note == note)
                {
                    remove_note(%tri_stack, note);
                    if(tri_stack.pointer > 1)
                    {
                        tri_voice_1_note = peek_last(&tri_stack);
                        note_to_dac(0, tri_voice_1_note, CV_1_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                    else
                    {
                        tri_voice_1_free = true;
                        gates_handle_triphonic_note_on_off(channel, 1, is_note_on);
                    }
                }
                else if(tri_voice_2_note == note)
                {
                    if(tri_stack.pointer > 1)
                    {
                        tri_voice_2_note = peek_last(&tri_stack);
                        note_to_dac(1, tri_voice_2_note, CV_2_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                    else
                    {
                        tri_voice_2_free = true;
                        gates_handle_triphonic_note_on_off(channel, 2, is_note_on);
                    }
                }
                else if(tri_voice_3_note == note)
                {
                    if(tri_stack.pointer > 1)
                    {
                        tri_voice_3_note = peek_last(&tri_stack);
                        note_to_dac(2, tri_voice_3_note, CV_3_SCALE == V_OCT);
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                    else
                    {
                        tri_voice_3_free = true;
                        gates_handle_triphonic_note_on_off(channel, 3, is_note_on);
                    }
                }
                else
                {
                    remove_note(&tri_stack, note);
                }
            }
            break;
        }
    }
}

void push_note(struct stack_t *data, uint8_t note)
{
    if(stack->pointer < 10)
    {
        data->stack[data->pointer] = note;
        data->pointer++;
    }
    else
    {
        for(int i=0; i<9; i++)
        {
            data->stack[i] = data->stack[i+1];
        }
        data->stack[9] = note;
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

uint8_t peek_last_duo(struct stack_t *data, uint8_t voice_1_note, uint8_t voice_2_note)
{
    uint8_t note = data->stack[data->pointer - 1];
    if(note == voice_1_note || note == voice_2_note)
    {
        note = data->stack[data->pointer - 2];
    }
    return note;
}

uint8_t peek_last_tri(struct stack_t *data, uint8_t voice_1_note, uint8_t voice_2_note, uint8_t voice_3_note)
{
    uint8_t note = data->stack[data->pointer - 1];
    if((note == voice_1_note) || (note == voice_2_note) || (note == voice_3_note))
    {
        note = data->stack[data->pointer - 2];
    }
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
        if(i < 8)
        {
            data->stack[i] = data->stack[i+1];
            data->stack[i+1] = 0;
        }
    }
}