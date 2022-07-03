[GLOBAL gdt_flush]
gdt_flush:              ; Define the Functions
    mov eax , [esp+4]   ; Get the First Parameter which is the GDT pointer
    lgdt[eax]           ; Load the GDT using our pointer

    mov ax, 0x10        ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax          ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    jmp 0x08:gdt_flush_end
gdt_flush_end:
	ret