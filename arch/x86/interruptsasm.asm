; @ File : interrupt.s -- Contains interrupt service routine wrappers.
;
; This macro creates a stub for an ISR which does NOT pass it's own
; error code (adds a dummy errcode byte).
extern isrNum
extern isrErrCode
%macro ISR_NOERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts firstly. 
    push eax
    mov eax,%1
    mov [isrNum],eax 
    jmp isr_common_stub         ; Go to our common handler code.
%endmacro

; This macro creates a stub for an ISR which passes it's own
; error code.
%macro ISR_ERRCODE 1
  global isr%1
  isr%1:
    cli                         ; Disable interrupts. 
    mov [isrErrCode],eax             ; Push the interrupt number
    mov eax,%1
    mov [isrNum],eax
    jmp isr_common_stub
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 1
ISR_NOERRCODE 2
ISR_NOERRCODE 3
ISR_NOERRCODE 4
ISR_NOERRCODE 5
ISR_NOERRCODE 6
ISR_NOERRCODE 7
ISR_ERRCODE   8
ISR_NOERRCODE 9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_NOERRCODE 17
ISR_NOERRCODE 18
ISR_NOERRCODE 19
ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31

; In isr.cc
extern isr_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
isr_common_stub:
  pusha
  mov ax,ds
  push eax
  mov ax,0x10
  mov ds,ax
  mov es,ax
  mov fs,ax
  mov gs,ax
  call isr_handler
  add esp,4
  popa
  iret


;; IRQ Definitions
;; Define the IRQ Handlers used in idt.h
;;
;; This Macro Creates the Functions
extern irqNum
%macro IRQ 2
  [GLOBAL irq%1]  ; Use the First Parameter
  irq%1:
    cli
    push eax
    mov eax,%2
    mov [irqNum],eax
    jmp irq_common_stub
%endmacro

;; Define the Functions using the macro
IRQ	0	,	32
IRQ	1	,	33
IRQ	2	,	34
IRQ	3	,	35
IRQ	4	,	36
IRQ	5	,	37
IRQ	6	,	38
IRQ	7	,	39
IRQ	8	,	40
IRQ	9	,	41
IRQ	10	,	42
IRQ	11	,	43
IRQ	12	,	44
IRQ	13	,	45
IRQ	14	,	46
IRQ	15	,	47


; Called when an  interrupt is raised by the PIC
[EXTERN irq_handler]

;; Define irq_common_stub , similar to isr_common_stub
irq_common_stub:
  pusha
  mov eax,16
  mov ds,ax
  mov eax,0x20
  out 0x20,al
  call irq_handler
  popa
  pop eax
  mov ds,ax
  sti
  iret

   
[GLOBAL idt_flush]

idt_flush:
  mov eax , [esp+4]     ; Get the pointer to the idt struct
  lidt [eax]            ; Loads the IDT
  ret   

[GLOBAL syscall_irq]
extern syscall_handler
syscall_irq:
	cli
  push ds
  push es
  push fs
  push gs
  pusha
  push eax
  push ebx
  mov ax, 0x10  ; load the kernel data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  pop ebx
  pop eax
  push ebx
  push eax
	call syscall_handler
  add esp,8
  popa
  pop gs
  pop fs
  pop es
  pop ds
	sti
	iret
[GLOBAL timer_irq]
extern timer_handler
timer_irq:
	cli
	pusha
  mov ax,ds
  push eax
  mov ax, 0x10  ; load the kernel data segment descriptor
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
	mov eax,0x20
  out 0x20,al
	; Call C handler
	call timer_handler
  pop ebx
   mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
  popa
	sti
	iret
[GLOBAL keyboard_irq]
extern keyboard_handler
keyboard_irq:
	cli
	mov ax,ds
	pusha
	push eax
	; Switch the kernel data segment
	mov ax,0x10
	mov ds,ax
	; Tell PIC for end of interrupt
	mov eax,0x20
	out 0x20,al
	; Call C handler
	call keyboard_handler
	; Pop's all poped registers
	pop eax
	mov ds,ax
	popa
	; Return last data segment
	sti
	iret