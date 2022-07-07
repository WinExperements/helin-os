#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
void syscall_handler(registers_t *regs) {
	printf((char *)regs->ebx);
}