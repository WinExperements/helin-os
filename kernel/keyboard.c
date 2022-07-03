#include <keyboard.h>
#include <arch.h>
#include <io.h>
#include <interrupts.h>
#include <x86/idt.h>
#include <terminal.h>
void keyboard_handler(int,int);
void keyboard_init() {
	interrupts_addHandler(0x21,keyboard_handler);
	while(io_readPort(0x64) & 0x1) {
		io_readPort(0x60);;
	}
	io_writePort(0x64,0xae);
	io_writePort(0x64,0x20);
	uint8_t status = (io_readPort(0x60) | 1)  & ~0x10;
	io_writePort(0x64,0x60);
	io_writePort(0x60,status);
	io_writePort(0x60,0xf4);
}
void keyboard_handler(int i,int err) {
	uint8_t key = io_readPort(0x60);
	printf("Key: %d\n",key);
}