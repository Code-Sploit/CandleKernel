#ifndef KEYBOARD_H
#define KEYBOARD_H

#define BACKSPACE 0x0E
#define ENTER 0x1C

#define SC_MAX 57

static char __kstd_keyboard_buffer[256];

void __kstd_enable_keyboard(void);

#endif
