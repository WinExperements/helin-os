#ifndef TYPEDEFS_H
#define TYPEDEFS_H
#define NULL 0
#define BLACK         0
#define BLUE          1
#define GREEN         2
#define CYAN          3
#define RED           4
#define MAGENTA       5
#define BROWN         6
#define LIGHT_GREY    7
#define DARK_GREY     8
#define LIGHT_BLUE    9
#define LIGHT_GREEN   10
#define LIGHT_CYAN    11
#define LIGHT_RED     12
#define LIGHT_MAGNETA 13
#define LIGHT_BROWN   14
#define WHITE         15
#define PRINTK_INFO 1
#define PRINTK_ERR 2
typedef char                     int8_t;
typedef unsigned char           uint8_t;
typedef short                   int16_t;
typedef unsigned short         uint16_t;
typedef int                     int32_t;
typedef unsigned int           uint32_t;
typedef long long int           int64_t;
typedef unsigned long long int uint64_t;
typedef uint32_t                 size_t;
typedef int s32;
typedef char s8;
static inline size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
struct multiboot_info {
	uint32_t flags;
	uint32_t low_mem;
	uint32_t high_mem;
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	struct {
		uint32_t num;
		uint32_t size;
		uint32_t addr;
		uint32_t shndx;
	} elf_sec;
	unsigned long mmap_length;
	unsigned long mmap_addr;
	unsigned long drives_length;
	unsigned long drives_addr;
	unsigned long config_table;
	unsigned long boot_loader_name;
	unsigned long apm_table;
	unsigned long vbe_control_info;
	unsigned long vbe_mode_info;
	unsigned long vbe_mode;
	unsigned long vbe_interface_seg;
	unsigned long vbe_interface_off;
	unsigned long vbe_interface_len;
	uint64_t framebuffer_addr;
  	uint32_t framebuffer_pitch;
  	uint32_t framebuffer_width;
 	uint32_t framebuffer_height;
  	uint8_t framebuffer_bpp;
  	#define FRAMEBUFFER_TYPE_INDEXED 0
	#define FRAMEBUFFER_TYPE_RGB     1
	#define FRAMEBUFFER_TYPE_EGA_TEXT     2
  	uint8_t framebuffer_type;
};
// OSDEV Wiki: Detecting memory(x86)
typedef struct memory_entry {
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
} memory_entry;
#define MMAP_AVAILABLE	1
#define MMAP_RESERVED	2
#define MMAP_ACPI	3
#define MMAP_NVS	4
#define MMAP_BADRAM	5
typedef enum {false,true} bool;
typedef struct mod_addr {
	uint32_t start;
	uint32_t end;
	uint32_t cmdline;
	uint32_t pad;
} mod_addr_t;
#endif
