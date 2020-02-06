#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 10
struct sigaction sa;
struct itimerval timer;
sigset_t set ; // Set for blocking signals
int alarm_fired = 0;

//setup singal handler
void signal_handler(){
    alarm_fired = 1;
}

void preempt_disable(void)
{
    //printf("Disabling preempt \n");
    sigemptyset(&set); // Set set to empty set
    sigaddset(&set, SIGVTALRM); // add SIGVT TO ALARM
    sigprocmask(SIG_BLOCK, &set, NULL);
    /* TODO Phase 4 */
}

void preempt_enable(void)
{
    //printf("Enabling preempt \n");
    sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void preempt_start(void)
{
    //printf("Starting Preempt \n");
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = &signal_handler;
    sigaction (SIGVTALRM, &sa, NULL);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = HZ*100;
    /* ... and every 250 msec after that. */
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = HZ*100;
    /* Start a virtual timer. It counts down whenever this process is
    executing. */
    setitimer (ITIMER_VIRTUAL, &timer, NULL);
}

void test_preempt_start() {
    /* Starting Pre-empt */
    preempt_start();
    /* Clock to check time */
    clock_t begin = clock();
    double time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    /* In the span of two seconds, if Alarm_Fired = 1, we're good */
    while(time_spent <= 2) {
        if(alarm_fired == 1) {
            printf("PreEmpt Start - Successful\n");
            alarm_fired = 0;
        }
        time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    } // Loop: For two seconds
    printf("----------------------\n");
} // test_preempt_start()

void test_preempt_disable() {
    /* Starting Pre-empt */
    preempt_start();
    /* Clock to check time */
    clock_t begin = clock();
    double time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    /* Should only fire once now */
    while(time_spent <= 2) {
        if(alarm_fired == 1) {
            printf("PreEmpt Disable - Successful\n");
            preempt_disable();
            alarm_fired = 0;
        }
        time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    } // Loop: For two seconds
    printf("----------------------\n");
} // test_preempt_disable()

void test_preempt_enable() {
    /* Starting Pre-empt */
    preempt_start();
    /* Clock to check time */
    clock_t begin = clock();
    double time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    /* Should only fire once now */
    while(time_spent <= 2) {
        if(alarm_fired == 1) {
            printf("PreEmpt Disable - Successful\n");
            preempt_disable();
            alarm_fired = 0;
        }
        time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    } // Loop: For two seconds

    /* Enabling the Alarm again */
    preempt_enable();
    /* Clock to check time */
    begin = clock();
    time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    /* Should start firing again */
    while(time_spent <= 2) {
        if(alarm_fired == 1) {
            printf("PreEmpt Enable - Successful\n");
            alarm_fired = 0;
        }
        time_spent = (double) (clock() - begin) / CLOCKS_PER_SEC;
    } // Loop: For two seconds
}

int main(void) {
    test_preempt_start();
    test_preempt_disable();
    test_preempt_enable();
}