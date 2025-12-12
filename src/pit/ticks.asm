[BITS 32]

section .data
global pic_timer_ticks

pic_timer_ticks: dd 0   ; 32-bit tick counter, starts at 0


section .text
global pic_timer_get_ticks
global increment_tick_counter

; void increment_tick_counter(void)
increment_tick_counter:
    inc dword [pic_timer_ticks]
    ret

; int pic_timer_get_ticks(void)
pic_timer_get_ticks:
    mov eax, [pic_timer_ticks]  ; return value in eax
    ret
