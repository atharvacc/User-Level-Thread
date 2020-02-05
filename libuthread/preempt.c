#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"


/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
struct sigaction sa;
struct itimerval timer;
sigset_t set ; // Set for blocking signals

//setup singal handler
void signal_handler(){
	printf("inside handler \n");
	uthread_yield();
}

void preempt_disable(void)
{
	printf("Disabling preempt \n");
	sigemptyset(&set); // Set set to empty set 
	sigaddset(&set, SIGVTALRM); // add SIGVT TO ALARM
	sigprocmask(SIG_BLOCK, &set, NULL);
	/* TODO Phase 4 */
}

void preempt_enable(void)
{
	printf("Enabling preempt \n");
	sigprocmask(SIG_UNBLOCK, &set, NULL);
}

void preempt_start(void)
{
	printf("Starting Preempt \n");
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
