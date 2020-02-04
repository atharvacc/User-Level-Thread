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
	int state; // 0 for READY, 1 RUNNING, 2 for BLOCKED, 3 for ZOMBIE
	int retVal; // Store return value
};

struct TCB *new_tcb; // Initialize a TCB
int cur_TID = 0; // Initialize cur_TID to 1, since parent has tid 0


queue_t READY ; // Ready to run
queue_t BLOCKED ; // Waiting for TID
queue_t ZOMBIE; // Exited and holds retval

void uthread_yield(void)
{
	if (READY == NULL){ // If nothing is in queue or it was never initialized
		return;
	}
	
	new_tcb->state = 0; // Set currently running state to READY
	struct TCB* currThread = new_tcb; // Create a COPY of the currently running thread
	struct TCB* nextThread;
	queue_enqueue(READY, currThread); // Add to ready queue
	queue_dequeue(READY, &nextThread); // Get next thread in queue
	printf("len of readyQuee now is %d\n", queue_length(READY));
	new_tcb = nextThread; //Get the next thread
	new_tcb-> state = 1; // Set state to running
	ucontext_t* curContext = &(currThread->context); // Get current Context
	ucontext_t* nextContext = &(nextThread->context); // Get next context
	uthread_ctx_switch(curContext, nextContext) ; //make the next thread start
	
}

uthread_t uthread_self(void)
{
	return new_tcb->TID;
}

/* Create a new thread with the function and args */
int uthread_create(uthread_func_t func, void *arg)
{
	if (READY == NULL){
		READY = queue_create();
		cur_TID++;
	}
	new_tcb = malloc(sizeof(struct TCB));
	new_tcb->TID = cur_TID;
	new_tcb->stack = uthread_ctx_alloc_stack();
	ucontext_t *contextPtr = &(new_tcb->context);
	int retval = uthread_ctx_init(contextPtr, new_tcb->stack, func, arg);
	if (retval == -1){
		return -1;
	}
	new_tcb->state = 0;
	queue_enqueue(READY, new_tcb);
	cur_TID++;
	return new_tcb->TID;
}

void uthread_exit(int retval)
{
	if (ZOMBIE == NULL){ // create zombie if we're in the first state
		ZOMBIE = queue_create();
	}
	printf("IN ZOMBIEEEE \n");
	new_tcb ->retVal = retval; // assign retval to that tcb
	new_tcb -> state = 4;
	queue_enqueue(ZOMBIE, new_tcb); // add to zombie que

}

int uthread_join(uthread_t tid, int *retval)
{
	if (BLOCKED == NULL){ // Create BLOCKED if not created yet
		BLOCKED = queue_create();
	}
	int len = queue_length(READY);
		
	while(queue_length(READY) != 0){
		printf("here\n");
		uthread_yield();
	}
	

	
	/* TODO Phase 3 */
	return 0;
}

int hello(void* arg)
{
	printf("Hello world!\n");
	return 0;
}

int main(void)
{
	uthread_t tid;
	tid = uthread_create(hello, NULL);
	printf("TID IS %d\n", tid);
	
	uthread_join(tid, NULL);

	return 0;
}





