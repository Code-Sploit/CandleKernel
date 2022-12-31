#include "keyboard.h"

#include "../console/console.h"
#include "../pio/pio.h"
#include "../isr/isr.h"

#include "../../lib/stdio.h"

const char *__kstd_keyboard_sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                         "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                         "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                         "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                         "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                         "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const char __kstd_keyboard_sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                         '7', '8', '9', '0', '-', '=', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                         'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                         'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                         'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

static void __kstd_keyboard_callback(registers_t *__regs)
{
    uint8 __scode = __kstd_inb(0x60);

    if (__scode > SC_MAX) return;

    if (__scode == BACKSPACE) {
        if (__kstd_is_backspace_valid(__kstd_keyboard_buffer))
        {
            __kstd_vga_backspace();
        }
    }
    else if (__scode == ENTER)
    {
        __kstd_vga_newline();
        
        __kstd_console_run(__kstd_keyboard_buffer);

        __kstd_keyboard_buffer[0] = '\0';

        kstd_write("root@candleos > ");
    }
    else
    {
        char __ch     = __kstd_keyboard_sc_ascii[(int) __scode];
        char __str[2] = {__ch, '\0'};

        __kstd_append_char_to_string(__kstd_keyboard_buffer, __ch);;

        __kstd_vga_putchar(__ch, ATTR_BYTE_TEXT_COLOR_STD);
    }
}

int __kstd_enable_keyboard(void)
{
    __kstd_register_interrupt_handler(IRQ1, __kstd_keyboard_callback);

    return 0;
}
