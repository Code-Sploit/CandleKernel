#ifndef CONSOLE_H
#define CONSOLE_H

#define __COMMAND_HALT "HALT"
#define __COMMAND_REBOOT "REBOOT"
#define __COMMAND_HELP "HELP"
#define __COMMAND_PRINT_BLOCKS "MPRINT"
#define __COMMAND_CLEAR "CLEAR"
#define __COMMAND_READ "CAT"

void __kstd_execute_shutdown(void);

void __kstd_console_run(char *__cmd);

#endif
