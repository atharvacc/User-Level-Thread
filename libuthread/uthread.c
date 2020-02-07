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

// Struct for TCB for each thread //
struct TCB {
    uthread_t TID;
    ucontext_t context;
	void *stack;
	int state; // 0 for READY, 1 RUNNING, 2 for BLOCKED, 3 for ZOMBIE
	int retVal; // Store return value
	int isBlocked; // Check if is blocked(NOTUSED)
	int *retValSave;
	uthread_t tidWait;
};

struct TCB *new_tcb; // Initialize a TCB
struct TCB *cur_tcb; // Currentlyt runnning TCB
int cur_TID = 0; // Initialize cur_TID to 1, since parent has tid 0


queue_t READY ; // Ready to run
queue_t BLOCKED ; // Waiting for TID
queue_t ZOMBIE; // Exited and holds retval


// Find if the arg matches TID wait. The TID wait for a thread is initialized when a thread is blocked //
static int findtidWait(void *data, void *arg)
{
    struct TCB  *a = (struct TCB*)data;
    int match = (int)(long)arg;
    if (a->tidWait == match) {
        return 1;
    }

    return 0;
} // findtidWait()

// Find if the arg matches the TID. This is used to find if the TID is in READY when joining, and tells us that something needs to be blocked //
static int find_tid(void *data, void *arg)
{
    struct TCB  *a = (struct TCB*)data;
    int match = (int)(long)arg;

    if (a->TID == match) {
        return 1;
    }

    return 0;
} // find_tid()

/* Unblocks if cur tcb's TID matches a TID wait in the BLOCK queue -> Assigns the retval to the collecting thread 
-> free up the exiting thread after destorying stack and enqueing the blocked process in the READY queue  */
int unblock()
{	
	struct TCB *curThread = cur_tcb;// create a copy of the current running thread
	void *blockedThread;

	if(queue_iterate(BLOCKED, findtidWait, (void*)(long)curThread->TID, &blockedThread) == 0) {
	    struct TCB *threadBlocked = (struct TCB*)blockedThread ;
	    threadBlocked->tidWait = -2;
	    queue_delete(BLOCKED, &threadBlocked);

	    if(threadBlocked->isBlocked == 1) {
		    queue_delete(ZOMBIE, &curThread); // delete it since it already unblocked the process it was waiting for.

		    if(threadBlocked->retValSave!=NULL) {
			    *(threadBlocked->retValSave) = curThread->retVal;
		    } // If: retvalsave is not null, then some retVal was used

		    threadBlocked->retVal = curThread->retVal;
	        uthread_ctx_destroy_stack(curThread->stack);
	        free(curThread);
	        queue_enqueue(READY, threadBlocked);
	        return 1;
	    } // If: threadBlocked is Blocked
	} // If: successfully execute queue_iterate()

	return 0;
} // unblock()



/* Used in join. This checks if the tid is present in the zombie thread. Thus we can extract the retval to the 
present thread, destroy the exited thread and then continue the current process */
int unblock_zombie(uthread_t tid)
{

	void *zombieThread;
	if (queue_iterate(ZOMBIE, find_tid, (void*)(long)tid, &zombieThread) == 0){
		struct TCB *threadZombie = (struct TCB*)zombieThread ;
		queue_delete(ZOMBIE, &threadZombie);
		if(threadZombie->state== 4){
			cur_tcb->retVal = threadZombie->retVal; 
			uthread_ctx_destroy_stack(threadZombie->stack);
			free(threadZombie);
			return 1;
		}
	
	}
	return 0;

}

/* Used in join to check if the TID is in READY QUEUE. If present, then we block the currently running process
 and yield to the next thread */
int in_ready(uthread_t tid)
{
	void *READYTHREAD;

	if(queue_iterate(READY, find_tid, (void*)(long)tid, &READYTHREAD) == 0){
		struct TCB *threadREADY = (struct TCB*)READYTHREAD;

		if(threadREADY->state == 0) {
			cur_tcb->tidWait = tid;
			cur_tcb->isBlocked = 1;
			cur_tcb->state = 2;
			queue_enqueue(BLOCKED, cur_tcb);
			return 1;
		} // If: threadReady's state is 0
	} // If: Successfully called queue_iterate()

	return 0;
} // in_ready()

/* Used in join to check if the TID is in BLOCKED QUEUE. If present, then we block the currently running process
 and yield to the next thread */
int in_blocked(uthread_t tid)
{
	void *BLOCKTHREAD;
	if(queue_iterate(BLOCKED, find_tid, (void*)(long)tid, &BLOCKTHREAD) == 0) {
		struct TCB *threadBLOCK = (struct TCB*)BLOCKTHREAD ;

		if(threadBLOCK->state == 2) {
            cur_tcb->tidWait = tid;
            cur_tcb->isBlocked = 1;
            cur_tcb->state = 2;
            queue_enqueue(BLOCKED, cur_tcb);

            return 1;
        } // If: threadBlock's state is 2
	} // If: Successfully called queue_iterate()

	return 0;
} // in_blocked()

void uthread_yield(void)
{
	if (READY == NULL || queue_length(READY) == 0) {
		return;
	} // If: Nothing is in queue or it was never initialized, return

	preempt_disable(); // Make sure YIELDing isn't affected by preempt
	cur_tcb->state = 0; // Set currently running state to READY
	struct TCB* currThread = cur_tcb; // Create a COPY of the currently running thread
	struct TCB* nextThread;

	if(currThread->isBlocked != 1){
		queue_enqueue(READY, (void**)currThread); // Add to ready queue
	} // If: currThread is not blocked

	queue_dequeue(READY, (void**)&nextThread); // Get next thread in queue
	cur_tcb = nextThread; //Get the next thread
	cur_tcb-> state = 1; // Set state to running
	ucontext_t* curContext = &(currThread->context); // Get current Context
	ucontext_t* nextContext = &(cur_tcb->context); // Get next context
	preempt_enable();
	uthread_ctx_switch(curContext, nextContext) ; //make the next thread start
	
} // uthread_yield()

uthread_t uthread_self(void)
{
	return cur_tcb->TID;
} // uthread_self()

/* Create a new thread with the function and args */
int uthread_create(uthread_func_t func, void *arg)
{
	if(READY == NULL) {
	    READY = queue_create();
		cur_tcb = malloc(sizeof(struct TCB));
		cur_tcb->TID = 0;
		cur_tcb->tidWait = -2;
		cur_tcb->isBlocked = 0;
		cur_tcb->retValSave = NULL;
		cur_TID++;
		preempt_start();
	} // If: haven't initialized main thread

	preempt_disable();
	new_tcb = malloc(sizeof(struct TCB));
	new_tcb->TID = cur_TID;
	new_tcb->stack = uthread_ctx_alloc_stack();
	new_tcb->tidWait = -2;
	new_tcb->isBlocked=0;
	new_tcb->retValSave = NULL;
	ucontext_t *contextPtr = &(new_tcb->context);

	int retval = uthread_ctx_init(contextPtr, new_tcb->stack, func, arg);
	if (retval == -1){
		return -1;
	} // If: failed to initialize context for thread

	new_tcb->state = 0; // READY State
	queue_enqueue(READY, new_tcb);
	cur_TID++;
	preempt_enable();

	return new_tcb->TID;
} // uthread_create()

void uthread_exit(int retval)
{
	preempt_disable();

	if(ZOMBIE == NULL){
		ZOMBIE = queue_create();
	} // If: in first state
	
	cur_tcb ->retVal = retval; // assign retval to that tcb
	cur_tcb -> state = 4;
	queue_enqueue(ZOMBIE, cur_tcb); // add to zombie queue
	struct TCB *curThread1 = cur_tcb;// create a copy of the current running thread

	/* If blocking something then unblock that and add to READY */
	unblock();
	struct TCB* nextThread;
	queue_dequeue(READY, (void**) &nextThread);
	cur_tcb = nextThread;
	preempt_enable();

	/* Start next thread from READY */
	uthread_ctx_switch(&(curThread1->context), &(cur_tcb->context));
	
} // uthread_exit()

int uthread_join(uthread_t tid, int *retval)
{
    /* Create BLOCKED if not created yet */
	if (BLOCKED == NULL) {
		BLOCKED = queue_create();
	} // If: Blocked queue hasn't been created
	
	if(tid == 0 || cur_tcb->TID == tid) {
		return -1;
	} // If: main, or for currently active process

	preempt_disable();

	/* Join Handling */
	if(unblock_zombie(tid) == 1) {
		if(retval != NULL) {
			*retval = cur_tcb->retVal;
		}

		preempt_enable();
		return 0;
	} // Thread was already done, thus check in ZOMBIE for TID == tid; assing retval to cur_tcb and continue execution

	else if (in_ready(tid) == 1) {
		if(retval != NULL) {
			cur_tcb->retValSave = retval;
		} // If retval is not null then we want to save  the returned thread a certain value

		preempt_enable();
		uthread_yield();
	} // Else if: it's in READY queue waiting to be executed, then block current one and execute next in READY QUEUE

	else if(in_blocked(tid) == 1) {
		if(retval != NULL) {
			*retval = cur_tcb->retVal;
		} // If: retval not NULL

		preempt_enable();
		return 0;
	} // Else if: it's in Blocked Queue
	
	else {
		preempt_enable();
		return -1;
	} // Else: Couldn't find in BLOCKED or READY and isn't presently Runnning

	preempt_enable();

	return 0;
} // uthread_join()

