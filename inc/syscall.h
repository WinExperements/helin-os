#ifndef SYSCALL_H
#define SYSCALL_H
#include <typedefs.h>
#include <x86/idt.h>
struct arch_regs {
	uint32_t edi,esi,ebp,esp,ebx,edx,ecx,eax,gs,fs,es,ds;
};
void syscall_handler(registers_t *regs);
#endif