#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include <uthread.h>

#define TEST_ASSERT(assert)			\
do {						\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {				\
		printf("PASS\n");		\
	} else	{				\
		printf("FAIL\n");		\
		exit(1);			\
	}					\
} while(0)

/* Global variables */
int thread2_on = 0;

int thread2(void* arg)
{
    thread2_on = 1;
    printf("In thread 2\n");
    return 0;
}

int thread1(void* arg)
{
    uthread_create(thread2, NULL);
    while(1) {
        if(thread2_on == 1)
            break;
    } // Infinite loop

    return 0;
}

int thread1_disable(void* arg)
{
    uthread_create(thread2, NULL);

    time_t start, end;
    time(&start);
    time(&end);

    while(difftime(end, start) < 10) {
        time(&end);
        uthread_create(NULL, NULL);
    }

    return 0;
}

void test_preempt_uncooperative(void *arg) {
    /* Test if preempt is switching operations even if one is non-cooperative */
    uthread_join(uthread_create(thread1, NULL), NULL);
    TEST_ASSERT(thread2_on == 1);
}

void test_preempt_disable(void *arg) {
    /* Test if preempt is disable during special uthread operations */
    uthread_join(uthread_create())
}

int main(void)
{
    test_preempt_uncooperative(NULL);
    test_preempt_disable(NULL);

    return 0;
}
