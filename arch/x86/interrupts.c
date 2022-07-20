#include <interrupts.h>
#include <io.h>
#include <x86/idt.h>
#include <x86/isr.h>
#include <mm.h>
#include <terminal.h>
#include <arch.h>
#include<syscall.h>
#include<x86/pic.h>
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;
isr_t interrupt_handlers[128];
void init_idt() {
  idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
    idt_ptr.base  = (uint32_t)&idt_entries;
    memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
    io_writePort(PIC_MASTER_COMMAND , 0x11);
  io_writePort(PIC_SLAVE_COMMAND , 0x11);
  io_writePort(PIC_MASTER_DATA , 0x20);
  io_writePort(PIC_SLAVE_DATA , 0x28);
  io_writePort(PIC_MASTER_DATA , 0x04);
  io_writePort(PIC_SLAVE_DATA , 0x02);
  io_writePort(PIC_MASTER_DATA , 0x01);
  io_writePort(PIC_SLAVE_DATA , 0x01);
  io_writePort(PIC_MASTER_DATA , 0x0);
  io_writePort(PIC_SLAVE_DATA , 0x0);

  // Initialise the IDT
  idt_set_gate( 0, (uint32_t)isr0 , 0x8, 0x8E);
  idt_set_gate( 1, (uint32_t)isr1 , 0x8, 0x8E);
  idt_set_gate( 2, (uint32_t)isr2 , 0x8, 0x8E);
  idt_set_gate( 3, (uint32_t)isr3 , 0x8, 0x8E);
  idt_set_gate( 4, (uint32_t)isr4 , 0x8, 0x8E);
  idt_set_gate( 5, (uint32_t)isr5 , 0x8, 0x8E);
  idt_set_gate( 6, (uint32_t)isr6 , 0x8, 0x8E);
  idt_set_gate( 7, (uint32_t)isr7 , 0x8, 0x8E);
  idt_set_gate( 8, (uint32_t)isr8 , 0x8, 0x8E);
  idt_set_gate( 9, (uint32_t)isr9 , 0x8, 0x8E);
  idt_set_gate( 10, (uint32_t)isr10 , 0x8, 0x8E);
  idt_set_gate( 11, (uint32_t)isr11 , 0x8, 0x8E);
  idt_set_gate( 12, (uint32_t)isr12 , 0x8, 0x8E);
  idt_set_gate( 13, (uint32_t)isr13 , 0x8, 0x8E);
  idt_set_gate( 14, (uint32_t)isr14 , 0x8, 0x8E);
  idt_set_gate( 15, (uint32_t)isr15 , 0x8, 0x8E);
  idt_set_gate( 16, (uint32_t)isr16 , 0x8, 0x8E);
  idt_set_gate( 17, (uint32_t)isr17 , 0x8, 0x8E);
  idt_set_gate( 18, (uint32_t)isr18 , 0x8, 0x8E);
  idt_set_gate( 19, (uint32_t)isr19 , 0x8, 0x8E);
  idt_set_gate( 20, (uint32_t)isr20 , 0x8, 0x8E);
  idt_set_gate( 21, (uint32_t)isr21 , 0x8, 0x8E);
  idt_set_gate( 22, (uint32_t)isr22 , 0x8, 0x8E);
  idt_set_gate( 23, (uint32_t)isr23 , 0x8, 0x8E);
  idt_set_gate( 24, (uint32_t)isr24 , 0x8, 0x8E);
  idt_set_gate( 25, (uint32_t)isr25 , 0x8, 0x8E);
  idt_set_gate( 26, (uint32_t)isr26 , 0x8, 0x8E);
  idt_set_gate( 27, (uint32_t)isr27 , 0x8, 0x8E);
  idt_set_gate( 28, (uint32_t)isr28 , 0x8, 0x8E);
  idt_set_gate( 29, (uint32_t)isr29 , 0x8, 0x8E);
  idt_set_gate( 30, (uint32_t)isr30 , 0x8, 0x8E);
  idt_set_gate( 31, (uint32_t)isr31 , 0x8, 0x8E);

  // Interrupts from 32 to 47 are reserved for the PIT
  idt_set_gate(32 , (uint32_t)scheduler_irq , 0x8 , 0x8E);
  idt_set_gate(33 , (uint32_t)irq1 , 0x8 , 0x8E);
  idt_set_gate(34 , (uint32_t)irq2 , 0x8 , 0x8E);
  idt_set_gate(35 , (uint32_t)irq3 , 0x8 , 0x8E);
  idt_set_gate(36 , (uint32_t)irq4 , 0x8 , 0x8E);
  idt_set_gate(37 , (uint32_t)irq5 , 0x8 , 0x8E);
  idt_set_gate(38 , (uint32_t)irq6 , 0x8 , 0x8E);
  idt_set_gate(39 , (uint32_t)irq7 , 0x8 , 0x8E);
  idt_set_gate(40 , (uint32_t)irq8 , 0x8 , 0x8E);
  idt_set_gate(41 , (uint32_t)irq9 , 0x8 , 0x8E);
  idt_set_gate(42 , (uint32_t)irq10 , 0x8 , 0x8E);
  idt_set_gate(43 , (uint32_t)irq11 , 0x8 , 0x8E);
  idt_set_gate(44 , (uint32_t)irq12 , 0x8 , 0x8E);
  idt_set_gate(45 , (uint32_t)irq13 , 0x8 , 0x8E);
  idt_set_gate(46 , (uint32_t)irq14 , 0x8 , 0x8E);
  idt_set_gate(47 , (uint32_t)irq15 , 0x8 , 0x8E);
  idt_set_gate(0x80,(uint32_t)syscall_irq,0x8,0x8E);
  idt_flush((uint32_t)&idt_ptr);
  memset(interrupt_handlers,0,256);
}
void idt_set_gate(uint8_t num,uint32_t base,uint16_t sel,uint8_t flags) {
  idt_entries[num].base_lo = base & 0xFFFF;
  idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

  idt_entries[num].sel     = sel;
  idt_entries[num].always0 = 0;
  // We must uncomment the OR below when we get to using user-mode.
  // It sets the interrupt gate's privilege level to 3.
  idt_entries[num].flags   = flags | 0x60;
}
void isr_handler() {
  printf("Exception %d, error code: %x\n",isrNum,isrErrCode);
  PANIC("CPU exception");
}
int irq_handler(registers_t *regs)
{
  // Send the EOI signal to the Slave
  // Slave handles the Interrupt from 40 to 47
    io_writePort(PIC_SLAVE_COMMAND , 0x20);

  // Send Reset Signal to Master
  io_writePort(PIC_MASTER_COMMAND , 0x20);

  // if Interrupt is already registered
  if (regs->int_no == 128) {
    syscall_handler(regs);
    return (int)regs;
  }
  if(interrupt_handlers[regs->int_no] != 0)
    {
      isr_t handler = interrupt_handlers[regs->int_no];
      handler(regs);
    }
    return (int)regs;
}
void interrupts_addHandler(uint8_t n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}
void interrupts_init() {
  init_idt();
}
