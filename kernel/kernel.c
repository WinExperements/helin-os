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
extern uint64_t endkernel;
void pic_handler(int i,int err) {
	printf("TIMER!!!!\n");
}
extern void kernel_main(struct multiboot_info *multiboot,uint32_t magic) {
	arch_init();
	if (magic != 0x2BADB002)
	{
		PANIC("Invalid multiboot!");
	}
	if (!(multiboot->flags >> 6 & 0x1))
	{
		PANIC("No memory map");
	}
	ppml_init(multiboot,0x200000);
	printf("Multiboot module description:\n");
	for (int i = 0; i < multiboot->mods_count; i++) {
		mod_addr_t *mod = (mod_addr_t *)multiboot->mods_addr+i;
		printf("Module start: %x, end: %x, command line: %s\n",mod->start,mod->end,mod->cmdline);
	}
	// Currently VMM needs to be rewriten, not use it
	// now process
	interrupts_addHandler(IRQ0,pic_handler);
	asm volatile("int $32");
}