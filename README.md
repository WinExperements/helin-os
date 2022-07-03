# helin-os
A simple hobby operation system written in C and assembly
# About
A hobby operation system written in C and assembly.<br>
Also kernel based on some open source projects!!!
# Features
Currently i implement microkernel for this operation system, because it's easily to realize. Yes microkernel slows that monolithic, but it's a my operation system :)<br>
Current list of features and released components:
- [x] Global description table
- [x] Interrupts
- [x] Memory Management, PMM
- [x] Multitasking(Bredan's multitasking see osdev wiki)
- [ ] Module Loading
- [x] Multiboot structure loading

# How to build?
First install the gcc cross-compiler,nasm,grub-common(if need to make an iso), then just fix paths word if you don't use Windows, some cross-compiller names and type "make"
# Bad news
- The Operation System may didn't support virtual memory and x86_64 CPU
- Doesn't support EFI(GRUB problems)
# TODO
- Syscall interface
- User mode
- Module and user program loading
