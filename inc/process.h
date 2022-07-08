#ifndef PROCESS_H
#define PROCESS_H
#include<typedefs.h>
#include<mm/vmm.h>
#include <x86/idt.h>
#define PROCESS_STACK_INIT  0xfffffff0
struct process {
	int stack;
	uint32_t virtAddr;
	int pid;
	bool used;
	struct process *next;
};
extern struct process *runningTask;
void process_init();
struct process *process_create(void (*)());
void process_delete(struct process *);
void process_kill(int);
void process_dump(struct process *);
void process_yield();
void process_schedule();
void process_test();
#endif
