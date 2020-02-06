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
}
int forever_loop(void* arg)
{
int i = 0;
while(1){
	printf(" is is %d \n", i);
	i = i +1;


}
}
int main(void)
{
	//uthread_t tid;
	uthread_t tid1;
	int a = 10;
	int *retval = &a;
	uthread_create(hello, NULL);
	tid1 = uthread_create(forever_loop, NULL);
	uthread_join(tid1, retval);
	printf("retval is : %d\n", *retval); // Check if hello world returns the correct value
	return 0;
}
