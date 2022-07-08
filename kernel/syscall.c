#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
void syscall_handler(registers_t *regs) {
	if (regs->eax > 1) {
		printf("No such syscall number: %d\n",regs->eax);
	} else {
		if (regs->eax == 1) {
			printf((char *)regs->ebx);
		}
	}
}