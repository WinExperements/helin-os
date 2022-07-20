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
#include <mstring.h>
extern uint64_t endkernel;
bool verbose;
extern void kernel_main(struct multiboot_info *multiboot) {
	arch_init();	// Architecture initialization
	printf("Helin OS kernel version 0.0.1 - User space and multitasking test\n");
	printf("Kernel build date: 20.07.2022\n");
	if (!(multiboot->flags >> 6 & 0x1))
	{
		PANIC("No memory map");
	}
	printf("No memory manager init, custom bootloader\n");
	ppml_init(multiboot,0x200000);
	printf("Remaping PIC and initalise the timer\n");
	io_writePort(0x21, 0xff);            // Disable all IRQs
   	io_writePort(0xa1, 0xff);             // Disable all IRQs
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
	printf("Task Manager init\n");
	process_init();
	printf("Enabling interrupts\n");
	// enable irq throught PIC
	io_writePort(0x21, 0x00);            // Enable all IRQs
   	io_writePort(0xa1, 0x00);             // Enable all IRQs
	uint32_t esp;
	asm volatile("mov %%esp, %0" : "=r"(esp));
	printf("TSS init\n");
	tss_set_stack(0x10,esp);
	if (!verbose) {
		terminal_clear();
	}
	printf("Now enable multitasking and others interrupts!\n");
	asm volatile("sti");
	printf("User mode\n");
	arch_switchToUser();
}
