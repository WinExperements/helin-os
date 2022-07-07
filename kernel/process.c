/* 	Process manager, used basekernel source code	*/
#include<process.h>
#include<mm/pmm.h>
#include<terminal.h>
#include <arch.h>
#define PUSH(tos,val) (*(--tos) = val)
struct process *runningTask;
struct process processList[1024];
int taskPidNext = 0;
int tasks;
void process_startup();
void process_addReady(struct process *prc);
void test() {
	while(1) {printf("A");}
}
void test2() {
	while(1) {printf("B");}
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
	PUSH(stack,(uint32_t)entryPoint); // the eip for the return of process_startup
	PUSH(stack,(uint32_t)process_startup); // eip that loaded aften function returned
	PUSH(stack,0);
	PUSH(stack,0);
	PUSH(stack,0);
	PUSH(stack,0);
	prc->stack = stack;
	// Because we need to rewrite VMM code, don't use it
	prc->virtAddr = arch_getVirtualAddress();
	asm volatile("cli");
	process_addReady(prc);
	asm volatile("sti");
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
	process_create(test);
	process_create(test2);
	printf("[process]: initialize done\n");
}
void process_dump(struct process *pr) {
	printf("Process %d dump\nPID: %d\nVMM address: %x\nRegisters address: %x\nEnd of dump\n",pr->pid,pr->pid,pr->virtAddr,pr->stack);
}
void process_yield() {
	PANIC("Not implemented");
}
void process_schedule() {
	if (tasks == 0) return;
	if (++taskPidNext > tasks) {
		taskPidNext = 0;
	}
	arch_switchTask(&processList[taskPidNext]);
}
void process_startup() {
	asm("sti");
}
void process_addReady(struct process *prc) {
	if (!prc) return;
	prc->next = NULL;
	processList[taskPidNext++] = *prc;
	tasks++;
}