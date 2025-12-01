[BITS 32]

section .text
global idle_task

; void idle_task(void)
idle_task:
.loop:
    sti             ; ensure interrupts enabled
    hlt             ; halt until next interrupt
    jmp .loop       ; repeat forever