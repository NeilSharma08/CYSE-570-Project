ORG 0x7c00
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

jmp short start
nop

; FAT16 Header
OEMIdentifier           db 'PEACHOS '
BytesPerSector          dw 0x200
SectorsPerCluster       db 0x80
ReservedSectors         dw 200
FATCopies               db 0x02
RootDirEntries          dw 0x40
NumSectors              dw 0x00
MediaType               db 0xF8
SectorsPerFat           dw 0x100
SectorsPerTrack         dw 0x20
NumberOfHeads           dw 0x40
HiddenSectors           dd 0x00
SectorsBig              dd 0x773594

; Extended BPB (Dos 4.0)
DriveNumber             db 0x80
WinNTBit                db 0x00
Signature               db 0x29
VolumeID                dd 0xD105
VolumeIDString          db 'PEACHOS BOO'
SystemIDString          db 'FAT16   '

%define MODEINFO_ADDR 0x5000
%define VBE_MODE 0x0118
%define VBE_LFB_BIT 0x4000
;%define MODEINFO_SRC      0x00005000    ; where INT 0x10 wrote ModeInfo
;%define MODEINFO_LEN      256
;%define MODEINFO_DEST     0x00101000    ; safe place inside kernel load area


start:
    jmp 0:step2

step2:
    cli ; Clear Interrupts
    mov ax, 0x00
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov es, ax

    mov sp, 0x7c00
    sti ; Enables Interrupts

.load_protected:
    cli
    
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov di, MODEINFO_ADDR

    ; Call VBE Get Mode Info (AX=0x4F01, CX=mode)
    mov ax, 0x4F01
    mov cx, VBE_MODE
    int 0x10
    ; AX = 0x004F on success
    cmp ax, 0x004F
    je .vbe_got_modeinfo

.vbe_failed:
    ; If VBE failed, just continue to protected mode (fallback to text)
    ; Optionally you can set a flag here at a known place in low memory.
    jmp .vbe_continue

.vbe_got_modeinfo:
    ; now set the mode with linear framebuffer bit
    mov ax, 0x4F02
    mov bx, VBE_MODE
    or bx, VBE_LFB_BIT
    int 0x10
    cmp ax, 0x004F
    jne .vbe_failed

    ; success — BIOS filled 256 bytes at MODEINFO_ADDR
    ; (leave it for kernel to read at physical 0x5000)

.vbe_continue:
    ; restore segments (still real mode) and continue to enable protected mode
    mov ax, 0x0000
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    ; (fall through to lgdt / CR0 enable)    

    lgdt[gdt_descriptor]
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:load32
    
; GDT
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0

; offset 0x8
gdt_code:     ; CS SHOULD POINT TO THIS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x9a   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

; offset 0x10
gdt_data:      ; DS, SS, ES, FS, GS
    dw 0xffff ; Segment limit first 0-15 bits
    dw 0      ; Base first 0-15 bits
    db 0      ; Base 16-23 bits
    db 0x92   ; Access byte
    db 11001111b ; High 4 bit flags and the low 4 bit flags
    db 0        ; Base 24-31 bits

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start-1
    dd gdt_start
 
 [BITS 32]
 load32:
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Enable the A20 line
    in al, 0x92
    or al, 2
    out 0x92, al

    ; For the loading...
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000


    call ata_lba_read
    
    ; copy VBE ModeInfo (256 bytes) from low memory to kernel area
    ;mov esi, MODEINFO_SRC    ; source physical (linear) addr
    ;mov edi, MODEINFO_DEST   ; destination physical (linear) addr
    ;mov ecx, MODEINFO_LEN
    ;cld
    ;rep movsb                ; copies 256 bytes

    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax, ; Backup the LBA
    ; Send the highest 8 bits of the lba to hard disk controller
    shr eax, 24
    or eax, 0xE0 ; Select the  master drive
    mov dx, 0x1F6
    out dx, al
    ; Finished sending the highest 8 bits of the lba

    ; Send the total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al
    ; Finished sending the total sectors to read

    ; Send more bits of the LBA
    mov eax, ebx ; Restore the backup LBA
    mov dx, 0x1F3
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send more bits of the LBA
    mov dx, 0x1F4
    mov eax, ebx ; Restore the backup LBA
    shr eax, 8
    out dx, al
    ; Finished sending more bits of the LBA

    ; Send upper 16 bits of the LBA
    mov dx, 0x1F5
    mov eax, ebx ; Restore the backup LBA
    shr eax, 16
    out dx, al
    ; Finished sending upper 16 bits of the LBA

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; Read all sectors into memory
.next_sector:
    push ecx

; Checking if we need to read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; We need to read 256 words at a time
    mov ecx, 256
    mov dx, 0x1F0
    rep insw
    pop ecx
    loop .next_sector
    ; End of reading sectors into memory
    ret

times 510-($ - $$) db 0
dw 0xAA55
