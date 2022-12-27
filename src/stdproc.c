#include "../lib/stdproc.h"
#include "../lib/stdlib.h"
#include "../lib/stdio.h"
#include "../lib/panic.h"

/*
Initialize the main (init) process
    PID: 1
*/

void __initialize_proc(void)
{
    if (__process_init != __PROCESS_UNINITIALIZED)
    {
        kstd_panic("__initialize_proc(): __process_init != __PROCESS_UNINITIALIZED");
    }

    __process_init = kstd_mem_malloc(sizeof(__proc));

    __process_init->__pnext   = __PROCESS_UNINITIALIZED;
    __process_init->__pid     = __PROCESS_COUNTER + 1;
    __process_init->__memsize = 512;

    __process_init->meta.__status   = __PROCESS_RUNNING;
    __process_init->meta.__priority = 1;

    __process_init->__paddr = kstd_mem_malloc(__process_init->__memsize);

    __PROCESS_COUNTER = (__PROCESS_COUNTER + 1);
}

/*
Initialize a new process
    PID: (PROCESS_COUNTER + 1)
*/

int __create_proc(int __status, int __priority)
{
    /* Create the process datastruct */

    __proc *__t = __process_init;
    __proc *__p = kstd_mem_malloc(sizeof(__proc));


    while (__t->__pnext != __PROCESS_UNINITIALIZED)
    {
        __t = __t->__pnext;
    }

    __p->__pid     = __PROCESS_COUNTER + 1;
    __p->__memsize = __PROCESS_MAXIMUM_MEMORY_SPACE;

    __p->meta.__status   = __status;
    __p->meta.__priority = __priority;

    __p->__paddr = kstd_mem_malloc(__t->__memsize);
    __p->__pnext = __PROCESS_UNINITIALIZED;

    __t->__pnext = __p;

    __PROCESS_COUNTER = (__PROCESS_COUNTER + 1);

    return __p->__pid;
}

void __print_procs(void)
{
    __proc *__t = __process_init;

    while (__t != __PROCESS_UNINITIALIZED)
    {
        char *__pid;
        char *__msz;
        char *__sts;
        char *__pri;

        kstd_itoa(__pid, 10, __t->__pid);
        kstd_itoa(__msz, 10, __t->__memsize);
        kstd_itoa(__sts, 10, __t->meta.__status);
        kstd_itoa(__pri, 10, __t->meta.__priority);

        kstd_write("PID: ");
        kstd_write(__pid);
        kstd_write(" MSZ: ");
        kstd_write(__msz);
        kstd_write(" STS: ");
        kstd_write(__sts);
        kstd_write(" PRI: ");
        kstd_write(__pri);
        kstd_write("\n");

        __t = __t->__pnext;
    }
}
