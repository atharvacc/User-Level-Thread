
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>

/* Test if calling uthread_yield, when READY was never initialzied returns without doing anything */
int main(void)
{
	uthread_yield();
	printf("done");
	return 0;
}
