/*
All code in graphics.c is our addition to the kernel. The code includes kernel 
system functions called by the interrupts service routine. 
The implementation of these system functions improves graphics implementation 
by increasing the speed of the game play and visual displays.  
*/
#include <stddef.h>
#include <stdint.h>
#include "graphics.h"
#include "idt/idt.h"
#include "task/task.h"
#include "isr80h/isr80h.h" 
#include "isr80h/misc.h"
#include "kernel.h"  
#include "status.h"  

uint32_t* fb = 0;

// initializes graphics mode in the kernel 
void graphics_init() {
    struct vbe_mode_info* vbe = (struct vbe_mode_info*)(0x00005000);
    vbe = (struct vbe_mode_info*) 0x5000;
    fb = (uint32_t*) vbe->physbase;
}

//System calls that run as interrupt service routines
void putpixel(int x, int y, uint32_t color){
    //uint32_t* fb = (uint32_t*)0xFD000000;
    //print("printing interrupt pixel\n");

    //print(itoa(color));
    //print("\n");
    uint32_t* pixel = (uint32_t*)((uint8_t*) fb + y * 3072 + x * 3);
    *pixel = color;
    //print(itoa((int)fb));
    //print("\n");

    return;
}

void sys_draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            putpixel(x + xx, y + yy, color);
        }
    }
}

void sys_draw_hline(int x_s, int y_s, int x_e, uint32_t color, int thickness) {
    for (int xx = x_s; xx <= x_e; xx++) {
        for (int yy = y_s - thickness + 1; yy < y_s + thickness; yy++) {
            putpixel(xx, yy, color);
        }
    }
}

void sys_draw_vline(int x_s, int y_s, int y_e, uint32_t color, int thickness) {
    for (int yy = y_s; yy <= y_e; yy++) {
        for (int xx = x_s - thickness + 1; xx < x_s + thickness; xx++) {
            putpixel(xx, yy, color);
        }
    }
}

void sys_draw_grid(int spacing) {
    for (int xx = 0; xx <= 1024; xx += spacing){
    	sys_draw_vline(xx, 0, 768, 0, 3);
    }
    
    for (int yy = 0; yy <= 768; yy += spacing){
        sys_draw_hline(0, yy, 1024, 0, 3);
    }
}

// Helper function for data processing - converts int to string.
char* itoa(int i)
{
    static char text[12];
    int loc = 11;
    text[11] = 0;
    char neg = 1;
    if (i >= 0)
    {
        neg = 0;
        i = -i;
    }

    while(i)
    {
        text[--loc] = '0' - (i % 10);
        i /= 10;
    }

    if (loc == 11)
        text[--loc] = '0';
    
    if (neg)
        text[--loc] = '-';

    return &text[loc];
}
