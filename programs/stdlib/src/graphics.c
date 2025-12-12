#include <stddef.h>
#include <stdint.h>
#include <graphics.h>

struct vbe_mode_info* vbe = (struct vbe_mode_info*)(0x00101000);
uint32_t* fb = 0;

void graphics_init() {
    fb = (uint32_t*) vbe->physbase;
}

static inline void putpixel(int x, int y, uint32_t color) {
    uint32_t* pixel = (uint32_t*)((uint8_t*) fb + y * 3072 + x * 3);
    *pixel = color;
}

void draw_rect(int x, int y, int w, int h, uint32_t color) {
    for (int yy = 0; yy < h; yy++) {
        for (int xx = 0; xx < w; xx++) {
            putpixel(x + xx, y + yy, color);
        }
    }
}

void draw_hline(int x_s, int y_s, int x_e, uint32_t color, int thickness) {
    for (int xx = x_s; xx <= x_e; xx++) {
        for (int yy = y_s - thickness + 1; yy < y_s + thickness; yy++) {
            putpixel(xx, yy, color);
        }
    }
}

void draw_vline(int x_s, int y_s, int y_e, uint32_t color, int thickness) {
    for (int yy = y_s; yy <= y_e; yy++) {
        for (int xx = x_s - thickness + 1; xx < x_s + thickness; xx++) {
            putpixel(xx, yy, color);
        }
    }
}

void draw_grid(int spacing) {
    for (int xx = 0; xx <= 1024; xx += spacing){
    	draw_vline(xx, 0, 768, 0, 3);
    }
    
    for (int yy = 0; yy <= 768; yy += spacing){
        draw_hline(0, yy, 1024, 0, 3);
    }
}


