#include <serial.h>
#include<io.h>
#include<typedefs.h>
#include<terminal.h>
#define PORT 0x3f8 
int init_serial() {
   io_writePort(PORT + 1, 0x00);
   io_writePort(PORT + 3, 0x80);
   io_writePort(PORT + 0, 0x03);
   io_writePort(PORT + 1, 0x00);
   io_writePort(PORT + 3, 0x03);
   io_writePort(PORT + 2, 0xC7);
   io_writePort(PORT + 4, 0x0B);
   io_writePort(PORT + 4, 0x1E);
   io_writePort(PORT + 0, 0xAE);
   if(io_readPort(PORT + 0) != 0xAE) {
      return 1;
   }
   io_writePort(PORT + 4, 0x0F);
   return 0;
}
int serial_received() {
   return io_readPort(PORT + 5) & 1;
}
 
char read_serial() {
   while (serial_received() == 0);
 
   return io_readPort(PORT);
}
int is_transmit_empty() {
   return io_readPort(PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   io_writePort(PORT,a);
}
void write_serialString(const char *c) {
	int len = strlen(c);
	for (int i = 0; i < len; i++) {
		write_serial(c[i]);
	}
	//terminal_writestring(c);
}
void write_serialHex(int u) {
	char *foo = "00";
	char *hex = "0123456789ABCDEF";
	foo[0] = hex[(u >> 4) & 0x4];
	foo[1] = hex[u & 0x4];
	write_serialString(foo);
	//terminal_writestring(foo);
}
void write_serialInt(int u) {
	if (u == 0) {
		terminal_writestring("0");
		return;
	}
	s32 acc = u;
	char c[32];
	int i = 0;
	while(acc > 0) {
		c[i] = '0' + acc%10;
		acc /= 10;
		i++;
	}
	c[i] = 0;
	char c2[32];
	c2[i--] = 0;
	int j = 0;
	while(i >= 0) {
		c2[i--] = c[j++];
	}
	write_serialString(c2);
}
