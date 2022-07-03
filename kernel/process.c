/* 	Process manager, used basekernel source code	*/
#include<process.h>
#include<mm/pmm.h>
#include<terminal.h>
#include <arch.h>
#define PUSH(tos,val) (*(--tos) = val)
struct process *runningTask;
struct process processList[1024];
void process_startup();
void test() {
	for (int i = 0; i < 100; i++) {
		printf("%d",i);
	}
	arch_switchTask(&processList[0]);
}
struct process *process_allocateProcess() {
	for (int i = 0; i < 1024; i++) {
		if (!processList[i].used) {
			processList[i].used = true;
			processList[i].pid = i;
			processList[i].stack = (uint32_t)pmml_alloc(true)+PHYS_PAGE_SIZE; // set top of stack
			return &processList[i];
		}
	}
	return NULL;
}
struct process *process_create(void (*entryPoint)()) {
	struct process *prc = process_allocateProcess();
	if (prc == NULL) {
		printf("Warrning: maximum of processes are running\n");
		return NULL;
	}
	uint32_t *stack = (uint32_t *)prc->stack;
	PUSH(stack,0);
	PUSH(stack,(uint32_t)entryPoint);
	PUSH(stack,0);
	PUSH(stack,0);
	PUSH(stack,0);
	PUSH(stack,0);
	prc->stack = stack;
	// Because we need to rewrite VMM code, don't use it
	prc->virtAddr = arch_getVirtualAddress();
	return prc;
}
void process_init() {
	for (int i = 0; i < 1024; i++) {
		processList[i].used = false;
	}
	// create kernel task here
	processList[0].used = true;
	processList[0].stack = 0;
	processList[0].virtAddr = arch_getVirtualAddress();
	processList[0].pid = 0;
	processList[0].next = NULL;
	runningTask = &processList[0];
	printf("[process]: initialize done\n");
}
void process_dump(struct process *pr) {
	printf("Process %d dump\nPID: %d\nVMM address: %x\nRegisters address: %x\nEnd of dump\n",pr->pid,pr->pid,pr->virtAddr,pr->stack);
}
void process_yield() {
	PANIC("Not implemented");
}
void process_test() {
	// create new process
	struct process *p = process_create(test);
	if (p != NULL) {
		arch_switchTask(p);
		printf("Return!!");
	} else {
		printf("Warrning: process_create return NULL!\n");
	}
}
void process_startup() {
	asm("sti");
}