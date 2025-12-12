#include "misc.h"
#include "idt/idt.h"
#include "task/task.h"
#include "pit/ticks.h"
#include "task/idle.h"
#include "graphics/graphics.h"
#include "kernel.h"
#include "status.h"

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
    //print("received interput to print pixel\n");
    int x = frame->ebx;
    int y = frame->ecx;
    uint32_t color = frame->edx;
    //print(itoa(color));
    //print("\n");
    // Call the privileged kernel implementation and return the result
    putpixel(x, y, color);
    
    // Return value in EAX for the user mode??
    //return (void*)(intptr_t)result;
    return (void*)PEACHOS_ALL_OK;
}

void* isr80h_command15_draw_rect(struct interrupt_frame* frame){
    int x = frame->ebx;
    int y = frame->ecx;
    int w = frame->edx;
    int h = frame->esi; // Need more registers for args
    uint32_t color = frame->edi;

    sys_draw_rect(x, y, w, h, color); 

    return (void*)PEACHOS_ALL_OK;
}

void* isr80h_command16_draw_hline(struct interrupt_frame* frame){
    int x_s = frame->ebx;
    int y_s = frame->ecx;
    int x_e = frame->edx;
    uint32_t color = frame->edi;
    int th = frame->esi; // Need more registers for args
    
    sys_draw_hline(x_s, y_s, x_e, color, th); 

    return (void*)PEACHOS_ALL_OK;
}


void* isr80h_command17_draw_vline(struct interrupt_frame* frame){
    int x_s = frame->ebx;
    int y_s = frame->ecx;
    int y_e = frame->edx;
    uint32_t color = frame->edi;
    int th = frame->esi; // same as above
    
    sys_draw_vline(x_s, y_s, y_e, color, th); 

    return (void*)PEACHOS_ALL_OK;
}

void* isr80h_command18_draw_grid(struct interrupt_frame* frame){
    int space = frame->ebx;
    
    sys_draw_grid(space); 

    return (void*)PEACHOS_ALL_OK;
}
