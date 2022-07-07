#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
void syscall_handler(int eax,int ebx) {
	if (eax == 1) {
		printf((char *)ebx);
	}
}