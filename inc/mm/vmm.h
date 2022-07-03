#ifndef MM_VMM_H
#define MM_VMM_H
#include<typedefs.h>
#include <x86/idt.h>
#define PAGE_SIZE 4096

#define PAGE_FLAG_USER        0
#define PAGE_FLAG_KERNEL      1
#define PAGE_FLAG_EXISTS      0
#define PAGE_FLAG_ALLOC       2
#define PAGE_FLAG_READONLY    0
#define PAGE_FLAG_READWRITE   4
#define PAGE_FLAG_NOCLEAR     0
#define PAGE_FLAG_CLEAR       8
#define MMU_PAGE_TABLE_ENTRIES_COUNT 1024
#define MMU_KERNEL_PAGES_COUNT 768
struct page {
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_supervisor : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t zero : 1;
    uint8_t page_size : 1;
    uint8_t ignored : 1;
    uint8_t available : 3;
    uint32_t page_table_addr : 20; /* 4Kb */
} __attribute__((packed));

struct page_table_entry_t {
    uint8_t present : 1;
    uint8_t read_write : 1;
    uint8_t user_supervisor : 1;
    uint8_t write_through : 1;
    uint8_t cache_disabled : 1;
    uint8_t accessed : 1;
    uint8_t dirty : 1;
    uint8_t zero : 1;
    uint8_t global : 1;
    uint8_t available : 3;
    uint32_t page_phys_addr : 20; /* 4Kb | 4MB */
} __attribute__((packed));

void vmm_init();
void vmm_enable();
void vmm_pfault(int i,int err);
#endif
