ARCH=x86
CCOPTIONS = -m32 -ggdb -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wunused-parameter -Wint-to-pointer-cast -Wsign-compare -nostdlib -march=i486 -mtune=i486
ASPARAMS =
CCPATH = D:\gcc\bin
OBJECTS = arch\$(ARCH)\boot.o arch\$(ARCH)\terminal.o kernel\kernel.o arch\$(ARCH)\io.o arch\$(ARCH)\serial.o arch\$(ARCH)\arch.o arch\$(ARCH)\poweroff.o arch\$(ARCH)\gdt.o arch\$(ARCH)\gdtfl.o arch\x86\interrupts.o arch\x86\interruptsasm.o mm\mm.o mm\pmm.o mm\vmm_x86.o kernel\mstring.o kernel\process.o arch\$(ARCH)\switch.o arch\$(ARCH)\mmu.o kernel\syscall.o kernel\keyboard.o arch\x86\pic.o kernel\elf.o
%.o: %.c
	$(CCPATH)\i686-elf-gcc $(CCOPTIONS) -Iinc -c -o $@ $<
%.o: %.s
	$(CCPATH)\i686-elf-as --32 -g -o $@ $<
%.o: %.asm
	D:\nasm\nasm -f elf32 -F dwarf -g -o $@ $<
all: $(OBJECTS)
	$(CCPATH)\i686-elf-ld -melf_i386 -T arch\$(ARCH)\linker.ld -o kernel.bin $(OBJECTS)
	@echo Compiled
clean:
	del $(OBJECTS)
makeiso:
	cp kernel.bin iso/boot/grub
	grub-mkrescue -o myos.iso iso
runkernel:
	"qemu-system-i386" -cpu 486 -kernel kernel.bin -serial stdio -m 256M
run:
	qemu-system-i386 -cdrom myos.iso
bochs:
	bochs -f bochsout.bsx
