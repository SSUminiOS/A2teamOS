[org 0]
[bits 16]

jmp start

start:
    mov ax, 0x07C0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    mov si, msg
    call print_msg

    mov si, miniOS_info
    call print_msg

    mov si, PS_info
    call print_msg

    mov si, MM_info
    call print_msg

    mov si, GUI_info
    call print_msg

    mov si, team_info
    call print_msg

    jmp hang

print_msg:
    print_loop:
        lodsb
        or al, al
        jz done
        mov ah, 0x0E
        int 0x10
        jmp print_loop

    done:
        mov al, 0x0A
        mov ah, 0x0E 
        int 0x10
        mov al, 0x0D
        int 0x10
        ret

hang:
    jmp hang

msg db 'The current OS is miniOS!', 0
miniOS_info db 'miniOS Information:', 0
PS_info db '1. Process Scheduling: FCFS, SJF, SRTF, Round Robin', 0
MM_info db '2. Memory Management: First Fit, Best Fit', 0
GUI_info db '3. GUI: Login, Input Entry, Output Text, Scroll', 0
team_info db '[Team 2/KKJ, LJU, CSH, CDJ]', 0

times 510-($-$$) db 0
dw 0xAA55
