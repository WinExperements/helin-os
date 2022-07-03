#ifndef MM_PMM_H
#define MM_PMM_H
#define PHYS_PAGE_SIZE 4096
#include<typedefs.h>
void ppml_init(struct multiboot_info *info,uint32_t endkernel);
void *pmml_alloc(bool);
int pmml_free(void *addr);
void pmml_test();
int pmml_getMemorySize();
int pmml_getMaxBlocks();
#endif
