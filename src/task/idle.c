#include "process.h"
#include "task.h"
#include "kernel.h"
#include "pit/ticks.h"

/* Idle task is disabled in the kernel. 
* Causes qemu emulator to crash.
*/


extern void idle_task();


void idle_init()
{
    struct process* proc = 0;
    int res = process_load_for_slot(NULL, &proc, 0);
    if (res != 0) {
        panic("Failed to create idle process");
    }

    /* Sets the instruction pointer to hlt in asm.
    * This causes a privledge error in the kernel
    * qemu will crash */
    proc->task->registers.ip = (uint32_t)idle_task;
    
    // Mark this task as the idle task.
    proc->task->flags = TASK_FLAG_IDLE;
    
    // Ensure the idle task is always ready
    proc->task->state = TASK_READY;
}
