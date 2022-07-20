#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
#include <process.h>
extern void ktt();
void syscall_handler(registers_t *regs) {
	if (regs->eax > 3) {
		printf("No such syscall number: %d\n",regs->eax);
	} else {
		if (regs->eax == 1) {
			printf((char *)regs->ebx);
		} else if (regs->eax == 2) { // process exit
			process_exit(regs->ebx,0);
		} else if (regs->eax == 3) {
			process_kill(regs->ebx);
		} else if (regs->eax == 4) {
			//arch_switchTask((uint32_t)ktt);
		} 
	}
}
