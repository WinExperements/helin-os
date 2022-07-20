#include <keyboard.h>
#include <arch.h>
#include <io.h>
#include <interrupts.h>
#include <x86/idt.h>
#include <terminal.h>
#include<x86/pic.h>
void keyboard_handler(registers_t *regs);
void keyboard_init() {
    interrupts_addHandler(33,keyboard_handler);
	unsigned char kbb = 0;
   	while(((kbb = io_readPort(0x64)) & 1) == 1)
   	{
      		io_readPort(0x60);
   	}
	while(io_readPort(0x64) & 0x1) {
		io_readPort(0x60);
	}
	io_writePort(0x64,0xae);
	io_writePort(0x64,0x20);
	uint8_t status = (io_readPort(0x60) | 1)  & ~0x10;
	io_writePort(0x64,0x60);
	io_writePort(0x60,status);
	io_writePort(0x60,0xf4);
}
void keyboard_handler(registers_t *regs) {
	uint8_t key = io_readPort(0x60);
	if (key < 0x80)
	{
		switch(key)
        {
            case 0x02: printf("1"); break;
            case 0x03: printf("2"); break;
            case 0x04: printf("3"); break;
            case 0x05: printf("4"); break;
            case 0x06: printf("5"); break;
            case 0x07: printf("6"); break;
            case 0x08: printf("7"); break;
            case 0x09: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;

            case 0x10: printf("q"); break;
            case 0x11: printf("w"); break;
            case 0x12: printf("e"); break;
            case 0x13: printf("r"); break;
            case 0x14: printf("t"); break;
            case 0x15: printf("y"); break;
            case 0x16: printf("u"); break;
            case 0x17: printf("i"); break;
            case 0x18: printf("o"); break;
            case 0x19: printf("p"); break;

            case 0x1E: printf("a"); break;
            case 0x1F: printf("s"); break;
            case 0x20: printf("d"); break;
            case 0x21: printf("f"); break;
            case 0x22: printf("g"); break;
            case 0x23: printf("h"); break;
            case 0x24: printf("j"); break;
            case 0x25: printf("k"); break;
            case 0x26: printf("l"); break;

            case 0x2C: printf("z"); break;
            case 0x2D: printf("x"); break;
            case 0x2E: printf("c"); break;
            case 0x2F: printf("v"); break;
            case 0x30: printf("b"); break;
            case 0x31: printf("n"); break;
            case 0x32: printf("m"); break;
            case 0x33: printf(","); break;
            case 0x34: printf("."); break;
            case 0x35: printf("-"); break;

            case 0x1C: printf("\n"); break;
            case 0x39: printf(" "); break;
            case 0xe: {
                terminal_writeXY(' ',terminal_getX(),terminal_getY());
            } break;
            case 0x38: {
                terminal_clearWithColor(BLUE,WHITE);
                printf("Cleared with ALT key\n");
            } break;
            default:
            {
		printf("Unknown key: %x\n",key);
                break;
            }
	}
	}
}
