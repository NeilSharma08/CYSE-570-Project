/*
All code in graphics.c is our addition to the kernel. The system calls improved the 
graphics implementation by using kernel system functions to drastically increase 
the speed of the game play and visual displays. Below are the system calls that 
optimize graphics mode. 
*/
#include <stddef.h>
#include <stdint.h>
#include <graphics.h>
#include "peachos.h"
#include <stdio.h>

// Displays pixel on screen.
void putpixel(int x, int y, uint32_t color) {   
    if(x < 0 || x > 1280 || y < 0 || y > 768){
    	return;
    }
    
    return peachos_put_pixel(x, y, color);
}

// Draws rectangle on screen 
void draw_rect(int x, int y, int w, int h, uint32_t color) {
    return peachos_draw_rect(x, y, w, h, color);
}

 // Draws horizontal line on screen
void draw_hline(int x_s, int y_s, int x_e, uint32_t color, int thickness) {
    return peachos_draw_hline(x_s, y_s, x_e, color, thickness); 
}

// Draws vertical line on screen.
void draw_vline(int x_s, int y_s, int y_e, uint32_t color, int thickness) {
    return peachos_draw_vline(x_s, y_s, y_e, color, thickness);
}

 // Draws grid on screen 
void draw_grid(int spacing) {
    return peachos_draw_grid(spacing);
}
