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
bool inUserMode,verbose;
void timer_handler(registers_t *regs) {
	if (inUserMode) {
		process_test();
	}
}
extern void kernel_main(struct multiboot_info *multiboot,uint32_t magic) {
	arch_init();	// Architecture initialization
	printf("Helin OS kernel version 0.0.1 - User space and multitasking test\n");
	printf("Kernel build date: 08.07.2022\n");
	printf("Framebuffer width: %d,height: %d, bpp: %d, address: %x\n",multiboot->framebuffer_width,multiboot->framebuffer_height,multiboot->framebuffer_bpp,multiboot->framebuffer_addr);
	if (magic != 0x2BADB002)
	{
		PANIC("Invalid multiboot!");
	}
	if (!(multiboot->flags >> 6 & 0x1))
	{
		PANIC("No memory map");
	}
	/* Check if user want to enable verbose mode of kernel */
	if (strcmp((char *)multiboot->cmdline,"-v")) {
		verbose = true;
	}
	printf("Memory managment init, skip VMM init\n");
	ppml_init(multiboot,0x200000);
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
	printf("Task Manager init\n");
	process_init();
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
	if (!verbose) {
		terminal_clear();
	}
	arch_switchToUser();
	//terminal_writestring("syscall\r\n");
	//process_init();	
	int a;
	int num = 1;
	char *p1 = "Hello from user space\n";
	asm volatile("int $0x80" : "=a" (a) : "0" (num), "b" ((int)p1));
	asm volatile("int $34");
	/* Only test, why we have the general protection fault error, because we trying to call the privileged instruction in un-privileged mode */
	/* now set the inUserMode to true to test the interrupt timer test */
	inUserMode = true;
}