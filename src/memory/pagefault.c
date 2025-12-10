#include "task/process.h"
#include "task/task.h"
#include "loader/formats/elfloader.h"
#include "memory/paging/paging.h"
#include "memory/heap/kheap.h"
#include "memory/memory.h"
#include "string/string.h"
#include "kernel.h" // For print() and panic()
#include <stdint.h>

/* Read CR2 register */
static inline uint32_t read_cr2()
{
    uint32_t val;
    __asm__ volatile("mov %%cr2, %0" : "=r"(val));
    return val;
}

/* ===== DEMAND PAGING PAGE FAULT HANDLER ===== */
void page_fault_handler_c(struct interrupt_frame* frame)
{
    uint32_t fault_addr = read_cr2();
    struct process* proc = process_current();

    struct process_elf_lazy_segment* seg = NULL;

    for (int i = 0; i < proc->elf_lazy_count; i++)
    {
        struct process_elf_lazy_segment* s = &proc->elf_lazy[i];
        if (fault_addr >= (uint32_t)s->vaddr_start &&
            fault_addr <  (uint32_t)s->vaddr_end)
        {
            seg = s;
            break;
        }
    }

    if (!seg)
    {
        print("FAULT ADDRESS: "); 
        hex(fault_addr); // Need to ensure itoa prints hex or decimal
        print("\n");

        print("Page fault outside ELF memory — killing process.\n");
        process_terminate(proc);
        task_next();
        return;
    }

    uint32_t page_vaddr = fault_addr & ~(PAGING_PAGE_SIZE - 1);
    uint32_t offset_in_seg = page_vaddr - (uint32_t)seg->vaddr_start;
    uint32_t file_offset = seg->file_offset + offset_in_seg;

    void* phys = kzalloc(PAGING_PAGE_SIZE);
    memset(phys, 0, PAGING_PAGE_SIZE);

    uint32_t max_bytes = 0;
    if (offset_in_seg < seg->file_size)
    {
        max_bytes = seg->file_size - offset_in_seg;
        if (max_bytes > PAGING_PAGE_SIZE)
            max_bytes = PAGING_PAGE_SIZE;

        memcpy(
            phys,
            (uint8_t*)elf_memory(proc->elf_file) + file_offset,
            max_bytes
        );
    }

    int flags = PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL;
    if (seg->flags & PF_W)
        flags |= PAGING_IS_WRITEABLE;

    paging_map(proc->task->page_directory, (void*)page_vaddr, phys, flags);
}
/* ===== END PAGE FAULT HANDLER ===== */
