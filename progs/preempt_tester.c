#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <uthread.h>

int thread3(void* arg)
{
    printf("Hello\n");
    time_t start, end;
    time(&start);
    do time(&end); while(difftime(end, start) <= 2.5);

    printf("thread%d\n", uthread_self());

    return 0;
}

int thread2(void* arg)
{
    uthread_create(thread3, NULL);
    uthread_yield();
    printf("thread%d\n", uthread_self());
    return 0;
}

int thread1(void* arg)
{
    uthread_create(thread2, NULL);
    uthread_yield();
    printf("thread%d\n", uthread_self());
    uthread_yield();
    return 0;
}

int main(void)
{
    uthread_join(uthread_create(thread1, NULL), NULL);
    return 0;
}
