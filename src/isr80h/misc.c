#include "misc.h"
#include "idt/idt.h"
#include "task/task.h"
#include "pit/ticks.h"
#include "task/idle.h"
#include "graphics/graphics.h"
#include "kernel.h"

void* isr80h_command0_sum(struct interrupt_frame* frame)
{
    int v2 = (int) task_get_stack_item(task_current(), 1);
    int v1 = (int) task_get_stack_item(task_current(), 0);
    return (void*)(v1 + v2);
}

void* isr80h_command12_get_ticks(struct interrupt_frame* frame)
{
    return (void*)(uint32_t)get_ticks();
}

void* isr80h_command13_put_pixel(struct interrupt_frame* frame)
{
    // Args for putpixel: (x, y, color)
    // Arg1 (x) is frame->ebx
    // Arg2 (y) is frame->ecx
    // Arg3 (color) is frame->edx
    print("received interput to print pixel\n");
    int x = frame->ebx;
    int y = frame->ecx;
    uint32_t color = frame->edx;
    print(itoa(color));
    print("\n");
    // Call the privileged kernel implementation and return the result
    int result = putpixel(x, y, color);
    
    // Return value is stored in EAX for the user mode
    return (void*)(intptr_t)result;
}
