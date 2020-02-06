#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include <uthread.h>

int thread3(void* arg)
{
    /* Clock to check time */
    clock_t begin = clock();
    double time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    /* In the span of two seconds, if Alarm_Fired = 1, we're good */
    while(time_spent <= 2) {
        time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    } // Loop: For two seconds

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
