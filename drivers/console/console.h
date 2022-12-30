#ifndef CONSOLE_H
#define CONSOLE_H

#define __COMMAND_HALT "HALT"
#define __COMMAND_REBOOT "REBOOT"

void __kstd_execute_shutdown(void);

void __kstd_console_run(char *__cmd);

#endif
