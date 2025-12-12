#include <stddef.h>
#include <stdint.h>
#include "graphics.h"
#include "idt/idt.h"
#include "task/task.h"
#include "isr80h/isr80h.h" 
#include "isr80h/misc.h"
#include "kernel.h"  
#include "status.h"  

//struct vbe_mode_info* vbe = (struct vbe_mode_info*)(0x00005000);
//uint32_t* fb = 0;
/*
void graphics_init() {
    fb = (uint32_t*) vbe->physbase;
    print(itoa((int)fb));
    print("\n");
    print(itoa((int)vbe));
    print("\n");
}
*/

int putpixel(int x, int y, uint32_t color){
    uint32_t* fb = (uint32_t*)0xFD000000;
    print("printing interrupt pixel\n");

    print(itoa(color));
    print("\n");
    uint32_t* pixel = (uint32_t*)((uint8_t*) fb + y * 3072 + x * 3);
    *pixel = color;
    print(itoa((int)fb));
    print("\n");

    return 0;
}

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
