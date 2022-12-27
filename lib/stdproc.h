#ifndef STDPROC_H
#define STDPROC_H

/* Process status */

#define __PROCESS_RUNNING 0
#define __PROCESS_SLEEPING 1
#define __PROCESS_WAITING 2

#define __PROCESS_UNINITIALIZED 0

/* Process data structure */

typedef struct PROC_STRUCT
{
    struct PROC_STRUCT *__pnext;

    union {
        int __status;
        int __priority;
    } meta;

    void *__paddr;

    int __memsize;
    int __pid;
} __attribute__((packed)) __proc;

#define __PROCESS_MAXIMUM_MEMORY_SPACE 512

/* Head process */

static __proc *__process_init = __PROCESS_UNINITIALIZED;

static int __PROCESS_COUNTER = 0;

/* Methods */

void __initialize_proc(void);

int __create_proc(int __status, int __priority);

void __print_procs(void);

#endif
