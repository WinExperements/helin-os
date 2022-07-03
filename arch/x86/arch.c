#include<arch.h>
#include<serial.h>
#include<terminal.h>
#include<x86/gdt.h>
#include <interrupts.h>
void gpf_exc(int,int);
void arch_init() {
	terminal_initialize();
	init_serial();
	// now init gdt
	gdt_init();
	interrupts_init();
	//interrupts_addHandler(13,gpf_exc);
}
void arch_reset() {
	__asm("jmp 0x0");
}
void panic(char *file,const char *funcName,const char *msg)
{
	//terminal_clearWithColor(BLUE,WHITE);
	terminal_setcolor(RED);
	terminal_setbackground(BLUE);
	printf("PANIC: %s:%s: %s\n",file,funcName,msg);
	for (;;) {}
}
void gpf_exc(int i,int err) {
	PANIC("General Purpose Exception\n");
}