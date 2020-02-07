/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

int hello(void* arg)
{
	printf("Hello world!\n");
	return 2;
} // hello()

int main(void)
{
	uthread_t tid;
	int a = 10;
	int *retval = &a;
	tid = uthread_create(hello, NULL);
	uthread_join(tid, retval);
	printf("retval is : %d\n", *retval); // Check if hello world returns the correct value
	return 0;
} // uthread_hello main()
