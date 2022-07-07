#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
void syscall_handler(const struct arch_regs *regs) {
	if (regs->eax > 1) {
		printf("No such syscall number: %d\n",regs->eax);
	} else {
		if (regs->eax == 1) {
			printf((char *)regs->ebx);
		}
	}
}