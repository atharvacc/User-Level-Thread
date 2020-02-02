#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"

struct TCB {
    uthread_t TID;
    ucontext_t context;
	void *stack;
	int state;
};

struct TCB *new_tcb; // Initialize a TCB
int cur_TID = 1; // Initialize cur_TID to 1, since parent has tid 0


void uthread_yield(void)
{
	/* TODO Phase 2 */
	
}

uthread_t uthread_self(void)
{
	return new_tcb->TID;
}

int uthread_create(uthread_func_t func, void *arg)
{
	new_tcb = malloc(sizeof(struct TCB));
	new_tcb->TID = cur_TID;
	new_tcb->stack = uthread_ctx_alloc_stack();
	ucontext_t *contextPtr = &(new_tcb->context);
	int retval = uthread_ctx_init(contextPtr, new_tcb->stack, func, arg);
	if (retval == -1){
		return -1;
	}

	cur_TID++;
	return new_tcb->TID;
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* TODO Phase 2 */
	/* TODO Phase 3 */
	return 0;
}

