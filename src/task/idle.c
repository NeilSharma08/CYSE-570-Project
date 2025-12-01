#include "process.h"
#include "task.h"
#include "kernel.h"

static void idle_process()
{
    while(1)
    {
        // Sleep for 1ms to yield the CPU
        process_sleep(task_current()->process, 1);
    }
}

void idle_init()
{
    struct process* proc = 0;
    int res = process_load_for_slot(NULL, &proc, 0);
    if (res != 0) {
        panic("Failed to create idle process");
    }

    // Set the instruction pointer to the idle process function
    proc->task->registers.ip = (uint32_t)idle_process;
    
    // Mark this task as the idle task.
    proc->task->flags = TASK_FLAG_IDLE;
    
    // Ensure the idle task is always ready
    proc->task->state = TASK_READY;
}
