// ok kernel start file
#include<terminal.h>
#include<serial.h>
#include<arch.h>
#include<mm.h>
#include<mm/pmm.h>
#include<process.h>
#include <mm/vmm.h>
#include <syscall.h>
#include <keyboard.h>
#include <io.h>
#include <interrupts.h>
#include <x86/gdt.h>
extern uint64_t endkernel;
extern void timer_handler() {
	//printf("TIMER!!!\n");
}
extern void kernel_main(struct multiboot_info *multiboot,uint32_t magic) {
	arch_init();	// Architecture initialization
	printf("Helin OS kernel version 0.0.1 - User space and syscalls test\n");
	printf("Kernel build date: 07.07.2022\n");
	if (magic != 0x2BADB002)
	{
		PANIC("Invalid multiboot!");
	}
	if (!(multiboot->flags >> 6 & 0x1))
	{
		PANIC("No memory map");
	}
	printf("Memory managment init, skip VMM init\n");
	ppml_init(multiboot,0x200000);
	// Currently VMM needs to be rewriten, not use it
	// now process
	//process_init();
	printf("Remaping PIC and initalise the timer\n");
	io_writePort(0x21, 0xff);            // Disable all IRQs
   	io_writePort(0xa1, 0xff);             // Disable all IRQs
   	interrupts_addHandler(32,timer_handler);
    io_writePort(0x20, 0x11);
    io_writePort(0xa0, 0x11);
    io_writePort(0x21, 0x20);
    io_writePort(0xa1, 0x28);
    io_writePort(0x21, 0x04);
    io_writePort(0xa1, 0x02);
    io_writePort(0x21, 0x01);
    io_writePort(0xa1, 0x01);
    printf("Keyboard driver init\n");
	keyboard_init();
	printf("Enabling interrupts\n");
	// enable irq throught PIC
	io_writePort(0x21, 0x00);            // Enable all IRQs
    io_writePort(0xa1, 0x00);             // Enable all IRQs
	asm volatile("sti");
	uint32_t esp;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	printf("TSS init\n");
	tss_set_stack(0x10,esp);
	printf("Switch to user mode...\n");
	arch_switchToUser();
	//terminal_writestring("syscall\r\n");
	//process_init();	
	int a;
	int num = 1;
	char *p1 = "Hello from user space\n";
	asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1));
}