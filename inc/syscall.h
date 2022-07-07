#ifndef SYSCALL_H
#define SYSCALL_H
#include <typedefs.h>
#include <x86/idt.h>
void syscall_handler(int,int);
#endif