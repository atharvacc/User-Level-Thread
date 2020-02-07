/*
 * Preemption Test
 *
 * Tests whether preemption is currently happen, where we would yield to
 * another thread even when a non-cooperative thread is hogging the process,
 * and is not yielding.
 *
 * Hence, the desired output of the test should be:
 *
 * "Inside Thread that wouldn't run without Preemption"
 */

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
    printf("Inside Thread that wouldn't run without Preemption\n");
    return 0;
} // thread2()

int thread1(void* arg)
{
    uthread_create(thread2, NULL);

    time_t start_time, current_time;
    time(&start_time);
    time(&current_time);

    while(difftime(current_time, start_time) < 5) {
        time(&current_time);
    } // 5 seconds loop

    return 0;
} // thread1()

int main(void)
{
    /* Test if preempt is switching operations even if one is non-cooperative */
    uthread_join(uthread_create(thread1, NULL), NULL);

    return 0;
} // preempt_tester()
