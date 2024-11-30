#include <stdio.h>
#include <signal.h>
#include <execinfo.h>
#include <stdlib.h>

void handle_segv(int signum)
{
    void *array[100];

    signal(signum, SIG_DFL); /* 还原默认的信号处理handler */
    size_t size = backtrace(array, 100);
    char **strings = (char **)backtrace_symbols(array, size);
    fprintf(stderr, "Launcher received SIG: %d Stack trace:\n", signum);

    for (size_t i = 0; i < size; i++)
    {
        fprintf(stderr, "%ld %s \n", i, strings[i]);
    }

    free(strings);
}

int showptr(int *p)
{
    int q = *p;
    return q;
}

int main()
{
    int *p = NULL;
    signal(SIGSEGV, handle_segv);
    signal(SIGABRT, handle_segv); 
    return showptr(p);
}