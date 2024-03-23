section .data
    pipe_name db "mypipe", 0

section .bss
    pipe_fd resq 1

section .text
    global _main

_main:
    ; Create the pipe
    mov rax, 0x2000005 ; sys_mkfifo
    lea rdi, [pipe_name]
    mov rsi, 0644o ; permissions (rw-r--r--)
    syscall

    ; Open the pipe for writing
    mov rax, 0x2000002 ; sys_open
    lea rdi, [pipe_name]
    mov rsi, 1 ; O_WRONLY
    mov rdx, 0
    syscall
    mov [pipe_fd], rax

    ; Close the pipe
    mov rax, 0x2000006 ; sys_close
    mov rdi, [pipe_fd]
    syscall

    ; Delete the pipe
    mov rax, 0x200000e ; sys_unlink
    lea rdi, [pipe_name]
    syscall

    ; Exit the program
    mov rax, 0x2000001 ; sys_exit
    xor rdi, rdi ; exit status 0
    syscall
