#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <uthread.h>

int thread2(void* arg)
{
    printf("thread%d\n", uthread_self());
    return 0;
}

int thread1(void* arg)
{
    uthread_create(thread2, NULL);

    time_t start, end;
    time(&start);
    do time(&end); while(difftime(end, start) < 2);

    printf("thread%d\n", uthread_self());

    return 0;
}

int main(void)
{
    uthread_join(uthread_create(thread1, NULL), NULL);
    return 0;
}
