/*
 * Thread creation and join test
 *
 * Tests the creation of multiples threads and the fact that when joining a
 * parent and a child, the child needs to die before the parent can be unblocked.
 *
 * Hence, the desired output of the test should be:
 *
 * thread3
 * thread2
 * thread1
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int thread3(void *arg) {
    printf("thread%d\n", uthread_self());
    return 0;
} // thread3()

int thread2(void *arg) {
    uthread_join(uthread_create(thread3, NULL), NULL);
    printf("thread%d\n", uthread_self());
    return 0;
} // thread2()

int thread1(void *arg) {
    uthread_join(uthread_create(thread2, NULL), NULL);
    printf("thread%d\n", uthread_self());

    return 0;
} // thread1()

int main(void) {
    uthread_join(uthread_create(thread1, NULL), NULL);
    return 0;
} // uthread_join main()
