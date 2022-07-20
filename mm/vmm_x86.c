/* X86 specific VMM code */
#include <mm/vmm.h>
#include <arch.h>
#include <mm/pmm.h>
#include <interrupts.h>
#include <terminal.h>
#include<mm.h>
struct page kdirectory __attribute__((aligned(4096)));
struct page_table_entry_t kpage_table[1024];
void vmm_init() {
	memset(&kdirectory,0,sizeof(struct page));
	kdirectory.zero = 1;
    kdirectory.accessed = 0;
    kdirectory.available = 0;
    kdirectory.cache_disabled = 0;
    kdirectory.ignored = 0;
    kdirectory.page_size = 0; /* 4KB */
    kdirectory.present = 1; /* kernel pages always in memory */
    kdirectory.read_write = 1; /* read & write */
    kdirectory.user_supervisor = 1; /* kernel mode pages */
    kdirectory.write_through = 1;
    kdirectory.page_table_addr = (size_t)kpage_table;
     for (int i = 0; i < pmml_getMaxBlocks(); ++i) {
        kpage_table[i].zero = 0;
        kpage_table[i].accessed = 0;
        kpage_table[i].available = 0;
        kpage_table[i].cache_disabled = 0;
        kpage_table[i].dirty = 0;
        kpage_table[i].global = 1;
        kpage_table[i].present = 1; /* kernel pages always in memory */
        kpage_table[i].read_write = 1; /* read & write */
        kpage_table[i].user_supervisor = 1; /* kernel mode pages */
        kpage_table[i].write_through = 1;
        kpage_table[i].page_phys_addr = (i * 4096); /* assume 4Kb pages */
        //printf("%x, %x\n",(i*4096),kpage_table[i].page_phys_addr << 12);
    }
    printf("Kernel directory address: %x\n",kdirectory);
    printf("Kernel page table: %x\n",kpage_table);
    // before enabling register exception handler here
    interrupts_addHandler(14,vmm_pfault);
   	//asm("int $0x14");
}
void vmm_pfault(registers_t *regs) {
	PANIC("PAGE FAULT!");
}
void vmm_enable() {
   asm volatile("mov %0, %%cr3":: "r"(kdirectory));
   uint32_t cr0;
   asm volatile("mov %%cr0, %0": "=r"(cr0));
   cr0 |= 0x80000000; // Enable paging!
   asm volatile("mov %0, %%cr0":: "r"(cr0));
}