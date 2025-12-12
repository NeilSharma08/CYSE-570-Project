#ifndef ISR80H_MISC_H
#define ISR80H_MISC_H

struct interrupt_frame;
void* isr80h_command0_sum(struct interrupt_frame* frame);
void* isr80h_command12_get_ticks(struct interrupt_frame* frame);
void* isr80h_command13_put_pixel(struct interrupt_frame* frame);
void* isr80h_command15_draw_rect(struct interrupt_frame* frame);
void* isr80h_command16_draw_hline(struct interrupt_frame* frame);
void* isr80h_command17_draw_vline(struct interrupt_frame* frame);
void* isr80h_command18_draw_grid(struct interrupt_frame* frame);
#endif