#include <stdint.h>

#define MODEINFO_PHYS 0x00005000 // Address for graphic mode

struct vbe_mode_info {
    uint16_t attributes;
    uint8_t  winA, winB;
    uint16_t granularity;
    uint16_t winsize;
    uint16_t segmentA, segmentB;
    uint32_t realFctPtr;
    uint16_t pitch;            // bytes per scanline (offset 0x10)
    uint16_t width;            // offset 0x12
    uint16_t height;           // offset 0x14
    uint8_t  wChar, yChar, planes, bpp, banks, memoryModel, bankSize, imagePages;
    uint8_t  reserved;

    uint8_t  redMask, redPos;
    uint8_t  greenMask, greenPos;
    uint8_t  blueMask, bluePos;
    uint8_t  rsvMask, rsvPos;
    uint8_t  directColorAttributes;

    uint32_t physbase;         // offset 0x28
    uint32_t reserved2;
    uint16_t reserved3;
} __attribute__((packed)); // ensures compiler does not add padding

// Protoypes of all fuctions in graphics.c
void graphics_init();
void putpixel(int x, int y, uint32_t color);
void draw_rect(int x, int y, int w, int h, uint32_t color);
void draw_hline(int x_s, int y_s, int x_e, uint32_t color, int thickness);
void draw_vline(int x_s, int y_s, int y_e, uint32_t color, int thickness);
void draw_grid(int spacing);
