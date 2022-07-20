#include<elf.h>
#include<mm.h>
#include<mm/pmm.h>
// The loading of ELF has been writen aften mltitasking fix
bool elf_check_file(Elf32_Ehdr *hdr) {
    return false;
}