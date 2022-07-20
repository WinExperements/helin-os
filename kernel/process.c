/* Preempective Process manager	*/
#include<process.h>
#include<mm/pmm.h>
#include<terminal.h>
#include <arch.h>
#define THREAD_MAX 1024
struct process processList[THREAD_MAX];
typedef struct _state {
	bool fswitch; 	// first switchk
	uint32_t threadndx; 	// index of thread
} state;
state scs;
/* If anybody have a question about why i add this start message, are know what is testing?	*/
void test() {
	printf_syscall("Task A is starting\n");
	while(1)
	{
		printf_syscall("Test\n");
	}
}
void test2() {
	printf_syscall("Task B is starting\n");
	while(1)
	{
		printf_syscall("Test2\n");
	}
}
void test3() {
	printf_syscall("Task C is starting\n");
	while(1) {
	}
	//asm volatile("sti");
}
void idle_task() {
	printf_syscall("IDLE start before loop");
	for (;;) {
		printf_syscall("IDLE\n");
	}
}
struct process *process_allocateProcess() {
	for (int i = 0; i < 1024; i++) {
		if (!processList[i].used) {
			processList[i].used = true;
			processList[i].pid = i;
			return &processList[i];
		}
	}
	return NULL;
}
struct process *process_create(void (*entryPoint)(),bool isUser,char *name) {
	struct process *p = process_allocateProcess();
	if (p->used) {
		printf("%s: %d\n",__func__,p->pid);
		stackFrame *frame = (pmml_alloc(true));
		frame->flags = 0x202;
		frame->cs    = (isUser ? 0x1b : 0x8);
 		frame->eip   = (uint32_t)entryPoint;
 		frame->ebp   = 0;
  		frame->esp   = 0;
  		frame->edi   = 0;
  		frame->esi   = 0;
  		frame->edx   = 0;
  		frame->ecx   = 0;
  		frame->ebx   = 0;
  		frame->eax   = 0;
  		frame->ds    = (isUser ? 0x23 : 0x10);
		frame->es = frame->ds;
		frame->fs = frame->ds;
		frame->gs = frame->ds;
		frame->ss = frame->ds;
		frame->usersp = (isUser ? ((int)pmml_alloc(true)+1024) : 0);
		p->esp = frame;
		p->name = name;
		p->kernelESP = (isUser ? ((int)pmml_alloc(true)) : 0);
		processList[p->pid].esp = frame;
		printf("Process %d, user stack: %x\n",p->pid,frame->usersp);
	}
	return p;	// not implemented
}
/* Issue found: task B doesn't starts in user mode */
void process_init() {
	for (int i = 0; i < 1024; i++) {
		processList[i].used = false;
	}
	struct process *p = process_create(idle_task,false,"idle");
	process_create(test,true,"test");
	process_create(test2,true,"pr");
	scs.fswitch = true;
	printf("[Process Manager]: Init done\n");
}
void process_dump(struct process *pr) {
	printf("Process %d dump\nPID: %d\nEnd of dump\n",pr->pid,pr->pid);
}
void process_yield() {
	PANIC("Not implemented");
}
stackFrame *process_schedule(stackFrame *stack) {
	// don't save context on first switch
	if (!scs.fswitch) {
		processList[scs.threadndx].esp = stack;
	}
	if (!scs.fswitch) {
		for (scs.threadndx = (scs.threadndx+1) & 0xf; !processList[scs.threadndx].used; scs.threadndx = (scs.threadndx +1) & 0xf);
	}
	struct process *p = &processList[scs.threadndx];
	tss_set_stack(0x10,p->kernelESP);
	scs.fswitch = false;
	return p->esp;
}
int process_getCurrentPID() {
	return scs.threadndx;
}
void process_exit(int pid,int exitCode) {
}
void process_kill(int pid) {
	process_exit(pid,-1);
}
void process_switchIdle() {
	//arch_executeTask(idleTask.esp);
}