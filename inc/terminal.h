#ifndef TERMINAL_H
#define TERMINAL_H
#include "typedefs.h"
void terminal_initialize(void);
void terminal_setcolor(uint8_t colo);
void terminal_setbackground(uint8_t back);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_writeInt(int u);
void terminal_writeHex(int num);
void printf(char *format,...);
int terminal_getX();
int terminal_getY();
int terminal_getBufferAddress();
void terminal_clearWithColor(uint8_t back,uint8_t cursor);
#endif
