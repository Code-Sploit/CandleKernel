#ifndef CONSOLE_H
#define CONSOLE_H

#define __COMMAND_HALT           "HALT"
#define __COMMAND_REBOOT         "REBOOT"
#define __COMMAND_HELP           "HELP"
#define __COMMAND_PRINT_BLOCKS   "MPRINT"
#define __COMMAND_CLEAR          "CLEAR"
#define __COMMAND_READ           "CAT"

static char *__KSTD_CONSOLE_HELP_MSG = "HELP:   Shows this window.\n"
                                       "HALT:   Halts the CPU.\n"
                                       "REBOOT: Reboots the system.\n"
                                       "MPRINT: Print memory block allocation table.\n"
                                       "CLEAR:  Clear the VGA buffer.\n"
                                       "CAT:    Read a file.\n";

void __kstd_execute_shutdown(void);

void __kstd_console_run(char *__cmd);

#endif
