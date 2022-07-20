#ifndef ARCH_H
#define ARCH_H
#include <process.h>
#define PANIC(msg) panic(__FILE__,__func__,msg)
void arch_init();
void arch_reset();
void arch_poweroff();
void panic(char *file,const char *funcName,const char *msg);
void arch_executeTask(int esp);
void arch_switchToUser();
#endif
