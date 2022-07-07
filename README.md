# About
A hobby operation system written in C and assembly.<br>
You can use source of this project :)<br>
Here the screnshot:<br>
![alt text](screenshot.png)
# Features
Currently i implement microkernel for this operation system, because it's easily to realize. Yes microkernel slows that monolithic, but it's a my operation system :)<br>
Current list of features and released components:
- [x] Global description table
- [x] Interrupts
- [x] Memory Management, PMM
- [x] Multitasking(Bredan's multitasking see osdev wiki)
- [ ] Module Loading
- [x] Multiboot structure loading
- [x] User space [in this file](arch/x86/gdtfl.asm)
- [x] Keyboard driver in user space
- [x] Working syscall interface, bugs see in [TODO](#TODO)

# How to build?
- Install any cross-compiler
- If you don't use Windows, then replace all '\\' to '/' in Makefile
- Type "make" or if you use Windows, type "mingw32-make"
# Bad news
- The Operation System may didn't support virtual memory and x86_64 CPU
- Doesn't support EFI(GRUB problems)
- Image creation(.iso) available only in linux
# TODO
- Fix passing arguments to syscall handler
- Module and user program loading