.text
.globl _main
_main:
    # 인터럽트 비활성화
    pushf
    cli

    # a += 3
    movl $0, %eax   # a 변수 초기화
    addl $3, %eax

    # b += 3
    movl $0, %ebx   # b 변수 초기화
    addl $3, %ebx

    # 인터럽트 활성화
    popf

    # 프로그램 종료
    mov $0x2000001, %rax    # _exit 시스템 호출 번호
    xor %rdi, %rdi          # 반환 값 0 설정
    syscall
