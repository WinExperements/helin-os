#include<terminal.h>
#include<io.h>
#include<serial.h>
#include<stdarg.h>
#include<arch.h>
uint16_t *video_memory;
uint8_t cursor_x;
uint8_t cursor_y;
int initialized;
uint8_t color;
uint8_t background;
bool replay;
void terminal_clear();
void terminal_initialize() {
	cursor_x = cursor_y = 0;
	video_memory = (uint16_t *)0xB8000;
	initialized = 1;
	color = WHITE;
	background = BLACK;
	terminal_clear();
}
void movecursor() {
	uint16_t pos = cursor_y * 80 + cursor_x;
	io_writePort(0x3D4,14);
	io_writePort(0x3D5,pos>>16);
	io_writePort(0x3D4,15);
	io_writePort(0x3D5,pos);
}
void scroll() {
	uint8_t attribute = ( BLACK << 4) | (WHITE & 0x0F);
  uint8_t blank = 0x20 | (attribute << 8);

  // A Standard Terminal size is 25 lines * 80 Coloumns
  if (cursor_y  >= 25)
    {
      // We are at the End of  the  Terminal so we have to move the entire text up one line
      int i;
      for(i = 0*80; i < 24*80; ++i)
        {
          video_memory[i] = video_memory[i+80];
        }

      // Now the Last Line is Left Blank , so we write 80 spaces using our blank character
      for (i = 24*80; i < 25*80; ++i)
        {
          video_memory[i] = blank;
        }

      // Set Y coordinate to last line
      cursor_y = 24;
    }
}
void putc(char ch,uint8_t back,uint8_t color) {
	uint8_t attribute =( back << 4) |(color & 0x0f);

  // Handle a backspace, by moving the cursor back one space
  if (ch == 0x08 && cursor_x)
    {
      cursor_x--;
    }

  // Handle a tab by increasing the cursor's X, but only to a point
  // where it is divisible by 8.
  else if (ch == 0x09)
    {
      cursor_x = (cursor_x+8) & ~(8-1);
    }

  // Handle carriage return
  else if (ch == '\r')
    {
      cursor_x = 0;
    }

  // Handle newline by moving cursor back to left and increasing the row
  else if (ch == '\n')
    {
      cursor_x = 0;
      cursor_y++;
    }

  // IF all the above text fails , print the Character
  if (ch >= ' ')
    {
      // Calculate the Address of the Cursor Position
      uint16_t *location = video_memory + (cursor_y * 80 + cursor_x);
      // Write the Bit into the cursor Postition
      *location = ch | (attribute << 8);
      cursor_x++;
    }

  // IF after all the printing we need to insert a new line
  if (cursor_x >= 80)
    {
      cursor_x = 0;
      cursor_y++;
    }

  // Scroll , or move the Cursor If Needed
  if (false) {
	write_serial(ch);
  }
  scroll();
  movecursor();
}
void terminal_writestring(const char *c) {
	int i =0;
	while(c[i]) {
		putc(c[i],background,color);
		i++;
	}
	//write_serialString(c);
}
void terminal_clear() {
	uint8_t attr = WHITE;
	uint8_t ch = 0x20;
	int i = 0;
  for (; i < 25*80; ++i)
    {
      video_memory[i] = ch | (attr << 8);
    }
    cursor_x = cursor_y = 0;
  movecursor();
}
void terminal_writeInt(int u) {
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
	terminal_writestring(c2);
}
void terminal_writeHex(int num) {
	uint32_t tmp;
	terminal_writestring("0x");
	char noZeroes = 1;
	int i;
	for (i = 28; i > 0; i-=4) {
		tmp = (num >> i) & 0xF;
		if (tmp == 0 && noZeroes != 0) continue;
		if (tmp >= 0xA) {
			noZeroes = 0;
			putc(tmp-0xA+'a',background,color);
		} else {
			noZeroes = 0;
			putc(tmp+'0',background,color);
		}
	}
	tmp = num & 0xF;
	if (tmp >= 0xA) {
		putc(tmp-0xA+'a',background,color);
	} else {
		putc(tmp+'0',background,color);
	}
} 
void terminal_setcolor(uint8_t colo) {
       color = colo;
}
void terminal_setbackground(uint8_t back) {
	background = back;
}
void printf(char *format,...) {
	va_list arg;
	va_start(arg,format);
	while (*format != '\0') {
		if (*format == '%') {
			if (*(format +1) == '%') {
				terminal_writestring("%");
			} else if (*(format + 1) == 's') {
				terminal_writestring(va_arg(arg,char*));
			} else if (*(format + 1) == 'c') {
				putc(va_arg(arg,int),background,color);
			} else if (*(format + 1) == 'd') {
				terminal_writeInt(va_arg(arg,int));
			} else if (*(format + 1) == 'x') {
				terminal_writeHex(va_arg(arg,int));
			} else if (*(format + 1) == '\0') {
				PANIC("printf: next char is null!");
			} else {
				PANIC("Unknown %");
			}
			format++;
		} else {
			putc(*format,background,color);
		}
		format++;
	}
	va_end(arg);
}
int terminal_getX() {
	return cursor_x;
}
int terminal_getY() {
	return cursor_y;
}
int terminal_getBufferAddress() {
	return 0xB8000;
}
void terminal_clearWithColor(uint8_t back,uint8_t cursor) {
	for (int i = 0; i < 80*25; i++) {
		putc(0x20,back,cursor);
	}
	cursor_x = 0;
	cursor_y = 0;
	movecursor();
}
void printf_syscall(const char *msg) {
	int res;
	int num = 1;
	asm volatile("int $0x80" : "=a" (res) : "0" (num), "b" ((int)msg));
}
void terminal_enableReplay(bool enable) {
	// Does we need to enable the replay to serial(Debug)
	replay = enable;
}
void terminal_writeXY(char c,uint8_t x,uint8_t y) {
	// very simple function
	int ox = cursor_x;
	int oy = cursor_y;
	cursor_x = x;
	cursor_y = y;
	putc(c,background,color);
	cursor_x = ox;
	cursor_y = oy;
}