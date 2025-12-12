/* All code in ticks.c, ticks.h, and ticks.asm is new. This is for the PIT, keeps track 
of the CPU cycles (or ticks). These system functions in C and assembly were added 
to implement the PIT. This file is the PIT code.*/

#include "ticks.h"
#include "kernel.h"
#include "idt/idt.h"
#include "task/task.h"
#include "task/process.h"
#include "io/io.h"
#include <stdint.h>

// Enables access to the tick counter
uint32_t get_ticks(void) {
    uint32_t count = pic_timer_get_ticks();   // calls the assembly function to get ticks
    return count;
}

 // Call back routine to handle interrupt request IR0.
void pic_timer_callback(struct interrupt_frame* frame)
{
    // Increment the assembly-defined tick counter
    increment_tick_counter();

    // Check for any processes that need to be woken up
    process_wake_up_pending();

    // Acknowledge the interrupt so we receive more.
    outb(0x20, 0x20);

    // Switch to the next task  
    task_next();
}

// Initialzes PIT in kernel.c
void pic_timer_init(void)
{
    idt_register_interrupt_callback(0x20, pic_timer_callback);
}

// sets PIT channel 0 to the requested frequency 
void pit_set_frequency(uint32_t hz)
{
    const uint32_t PIT_BASE = 1193182;
    uint16_t divisor = (uint16_t)(PIT_BASE / hz);

    outb(0x43, 0x36);               // command byte: channel 0, lo/hi, mode 3
    outb(0x40, (unsigned char)(divisor & 0xff));   // low byte
    outb(0x40, (unsigned char)((divisor >> 8) & 0xff)); // high byte
}
