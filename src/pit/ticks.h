#ifndef TICKS_H
#define TICKS_H

#include <stdint.h>

struct interrupt_frame;

/* Assembly-defined tick counter */
extern uint32_t pic_timer_ticks;

/* Functions */
void increment_tick_counter(void);   /* Implemented in assembly */
int pic_timer_get_ticks(void);       /* Implemented in assembly */
void pic_timer_init(void);            /* Implemented in C */
void pit_set_frequency(uint32_t hz); /* Implemented in C */
void pic_timer_callback(struct interrupt_frame* frame); /* Implemented in C */
uint32_t get_ticks(void);            /* Implemented in C */

#endif