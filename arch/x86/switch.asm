section .text
global arch_switchTask
global arch_getVirtualAddress
extern runningTask
TOS equ 0
VAS equ 4
arch_getVirtualAddress:
    mov eax,cr3
    ret
arch_switchTask:
    cli
    push ebx
    push esi
    push edi
    push ebp
    mov edi,[runningTask]
    mov [edi+TOS],esp
    mov esi,[esp+((4+1)*4)]
    mov [runningTask],esi
    mov esp,[esi+TOS]
    mov eax,[esi+VAS]
    mov ecx,cr3
    jmp .someVas
.someVas:
    pop         ebp
    pop         edi
    pop         esi
    pop         ebx
    sti
    ret