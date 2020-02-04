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
	int isBlocked; // Check if is blocked
};

struct TCB *new_tcb; // Initialize a TCB
struct TCB *cur_tcb; // Currentlyt runnning TCB
int cur_TID = 0; // Initialize cur_TID to 1, since parent has tid 0


queue_t READY ; // Ready to run
queue_t BLOCKED ; // Waiting for TID
queue_t ZOMBIE; // Exited and holds retval

void uthread_yield(void)
{
	if (READY == NULL){ // If nothing is in queue or it was never initialized
		return;
	}
	
	cur_tcb->state = 0; // Set currently running state to READY
	struct TCB* currThread = cur_tcb; // Create a COPY of the currently running thread
	struct TCB* nextThread;
	queue_enqueue(READY, (void**)currThread); // Add to ready queue
	queue_dequeue(READY, (void**)&nextThread); // Get next thread in queue
	//printf("len of readyQuee now is %d\n", queue_length(READY));
	cur_tcb = nextThread; //Get the next thread
	cur_tcb-> state = 1; // Set state to running
	ucontext_t* curContext = &(currThread->context); // Get current Context
	ucontext_t* nextContext = &(cur_tcb->context); // Get next context
	printf("new TID IS %d\n", nextThread->TID);
	printf("Curr TID IS %d\n", currThread->TID);
	uthread_ctx_switch(curContext, nextContext) ; //make the next thread start
	
}

uthread_t uthread_self(void)
{
	return cur_tcb->TID;
}

/* Create a new thread with the function and args */
int uthread_create(uthread_func_t func, void *arg)
{
	if (READY == NULL){
		READY = queue_create();
		cur_tcb = malloc(sizeof(struct TCB));
		cur_tcb->TID = 0;
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
	new_tcb->state = 0;//READY
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
	cur_tcb ->retVal = retval; // assign retval to that tcb
	cur_tcb -> state = 4;
	queue_enqueue(ZOMBIE, cur_tcb); // add to zombie que
	struct TCB* nextThread;
	if (queue_length(READY)!=0){
		queue_dequeue(READY, &nextThread);
		struct TCB *curThread = cur_TCB;
		cur_TCB = nextThread;
		uthread_ctx_switch(&(curThread->context), &(cur_TCB->context));
	}
	
}

int uthread_join(uthread_t tid, int *retval)
{
	if (BLOCKED == NULL){ // Create BLOCKED if not created yet
		BLOCKED = queue_create();
	}
	
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
	printf("Done\n");
	return 0;
}





