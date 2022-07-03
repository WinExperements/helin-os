/** Physical Page Allocator we don't use bitmaps or any others components **/

#include<mm/pmm.h>
#include<mm.h>
#include<serial.h>
#include<arch.h>
#include<terminal.h>
uint32_t main_memory_size;
uint8_t *bitmap;
uint32_t max_blocks,free_blocks,used_blocks;
void set_block_used(uint64_t index,bool used);
bool is_block_used(uint64_t blkN);
void ppml_init(struct multiboot_info *info,uint32_t bitmapAddr)
{
	main_memory_size = 0;
	uint64_t memaddr = 0;
	unsigned long i;
	for (i = 0; i < info->mmap_length; i+=sizeof(memory_entry))
	{
		memory_entry *en = (memory_entry *)(info->mmap_addr+i);
		if (en->type == 1 && en->len > main_memory_size) {
			main_memory_size = en->len;
			memaddr = en->addr;
		}
	}
	uint32_t memF = (main_memory_size)/PHYS_PAGE_SIZE;
	bitmap = (uint8_t *)bitmapAddr;
	memset(bitmap,0,memF);
	max_blocks = memF;
	used_blocks = memF/4096;
	free_blocks = max_blocks-used_blocks;
	for (i = 0; i < used_blocks; i++) {
		set_block_used(i,true);
	}
}
void *pmml_alloc(bool clear) {
	if (is_block_used(used_blocks+1)) {
		used_blocks++;
		return pmml_alloc(clear);
	}
	used_blocks++;
	set_block_used(used_blocks,true);
	void *addr = (void *)(used_blocks*PHYS_PAGE_SIZE);
	if (clear) {
		memset(addr,0,4096);
	}
	return addr;
}
int pmml_free(void *addr)
{
	int index = (int)addr/PHYS_PAGE_SIZE;
	if (!is_block_used(index)) {
		printf("pmml_free: given address are not allocated\n");
		return false;
	}
	set_block_used(index,false);
	memset(addr,0,4096);
	if (is_block_used(index))
		PANIC("Free, bitmap set fail");
	return true;
}
void pmml_test() {
	for (int i = 0; i < max_blocks; i++) {
		void *addr = pmml_alloc(1);
		//printf("Page address: %x, index in bit map: %d\n",(int)addr,(int)addr/4096);
		if (!pmml_free(addr))
			PANIC("Free error");
	}
	printf("Test finished\n");
}
void set_block_used(uint64_t index,bool used) {
	uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    bitmap[byteIndex] &= ~bitIndexer;
    if (used){
        bitmap[byteIndex] |= bitIndexer;
    }
	//printf("%s: block data: %x\n",__func__,bitmap[byteIndex]);
}
bool is_block_used(uint64_t index) {
	uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if ((bitmap[byteIndex] & bitIndexer) > 0){
        return true;
    }
    return false;
}
int pmml_getMemorySize() {
	return main_memory_size/1024/1024;
}
int pmml_getMaxBlocks() {
	return max_blocks;
}