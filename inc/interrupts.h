#ifndef INTERRUPTS_H
#define INTERRUPTS_H
#include "typedefs.h"
typedef void (*interrupt_handler_t) (int intr, int code);
void interrupts_init();
void interrupts_addHandler(uint8_t,interrupt_handler_t handler);
void interrupts_block();
void interrupts_unblock();
#endif
