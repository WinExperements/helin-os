#include <syscall.h>
#include <interrupts.h>
#include <terminal.h>
void syscall_init() {
	interrupts_addHandler(0x80,syscall_handler);
}
void syscall_handler() {
	printf("SYSCALL!\n");
}