#ifndef PEACHOS_H
#define PEACHOS_H
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>


struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};


void print(const char* filename);
int peachos_getkey();

void* peachos_malloc(size_t size);
void peachos_free(void* ptr);
void peachos_putchar(char c);
int peachos_getkeyblock();
void peachos_terminal_readline(char* out, int max, bool output_while_typing);
void peachos_process_load_start(const char* filename);
struct command_argument* peachos_parse_command(const char* command, int max);
void peachos_process_get_arguments(struct process_arguments* arguments);
int peachos_system(struct command_argument* arguments);
int peachos_system_run(const char* command);
void peachos_exit();

// Add functions below were added to the kernel. These are implmented in peachos.asm
void peachos_sleep(int seconds);
void peachos_wake();
unsigned int peachos_get_ticks();
unsigned int peachos_get_idle_ticks();
void peachos_put_pixel(int x, int y, uint32_t color);
void peachos_draw_rect(int x, int y, int w, int h, uint32_t color);
void peachos_draw_hline(int x_s, int y_s, int x_e, uint32_t color, int thickness); 
void peachos_draw_vline(int x_s, int y_s, int x_e, uint32_t color, int thickness);
void peachos_draw_grid(int spacing);
#endif