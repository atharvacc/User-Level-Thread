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

#define NUM_THREAD_STATE 5

/* TODO Phase 2 */
/* Global variables */
int main_thread_initialized = 0; // 0: Main thread not initialized, 1: Initialized
queue_t tcb_queue[5]; // 0: Init, 1: Ready, 2: Running, 3: Waiting, 4: Finished
int current_thread_id = 0; // 0: is reserved for Main Thread

struct TCB {
    uthread_t thread_id;  // TCB's identification
    void *thread_stack; // Stores instructions to be run
    uthread_ctx_t thread_context; // Stores backup CPU registers, functions, and arguments
    int thread_state; // 0: Main thread, 1: Ready, 2: Running, 3: Waiting, 4: Finished

}; // Thread Control Block

void uthread_yield(void)
{
	/* TODO Phase 2 */
    struct TCB *ready_to_run_tcb = NULL, *run_to_ready_tcb = NULL;

    /* Enqueue first Ready Thread to Queue[Running] */
    if(queue_dequeue(tcb_queue[READY_STATE], ready_to_run_tcb) == 0)
        ready_to_run_tcb->thread_state = RUNNING_STATE;
    queue_enqueue(tcb_queue[RUNNING_STATE], ready_to_run_tcb);

    /* Dequeue first Running Thread to Queue[Ready] */
    if(queue_dequeue(tcb_queue[RUNNING_STATE], run_to_ready_tcb) == 0)
        run_to_ready_tcb->thread_state = READY_STATE;
    queue_enqueue(tcb_queue[READY_STATE], run_to_ready_tcb);

    /* Switch from Running's context to Ready's context */
    uthread_ctx_switch(run_to_ready_tcb, ready_to_run_tcb);

} // uthread_yield()

uthread_t uthread_self(void)
{
    /* TODO Phase 2 */
	return tcb_queue[RUNNING_STATE].head->thread_id;
} // uthread_self()

int uthread_create(uthread_func_t func, void *arg)
{
    /* TODO Phase 2 */
	/* Initialize Main thread if it is not initialized */
	if(main_thread_initialized == 0) {
	    init_main_thread();
	    main_thread_initialized = 1;
	} // If: Main thread not initialized

	/* Create a runnable thread to execute 'func' with argument 'arg' */
	struct TCB *tcb = (struct TCB*)malloc(sizeof(struct TCB)); // Allocate space for TCB
	if(tcb == NULL) {return -1;} // If: Failed to malloc space for 'tcb'
	tcb->thread_id = ++current_thread_id; // Add 1 to get new thread_id
    tcb->thread_stack = uthread_ctx_alloc_stack(); // Allocate space for thread's stack
    uthread_ctx_init(&tcb->thread_context, tcb->thread_stack, func, arg); // Saves the context
    tcb->thread_state = READY_STATE; // Setting thread's state to Ready

	/* Add TCB to Ready/Runnable Queue */
	queue_enqueue(tcb_queue[READY_STATE], tcb);

	return tcb->thread_id;
} // uthread_create()

int init_main_thread()
{
    /* Creating main TCB */
    struct TCB *main_tcb = (struct TCB*)malloc(sizeof(struct TCB)); // Allocate space for main TCB
    if(main_tcb == NULL) {return -1;} // If: Failed to malloc space for 'main_tcb'
    main_tcb->thread_id = 0; // Set thread_id = the latest thread_id, then add 1 to current_thread_id
    main_tcb->thread_stack = NULL; // Doesn't need stack space
    // FIXME: main_thread's context does not need to be initialized (Piazza @194)
    main_tcb->thread_state = RUNNING_STATE; // Main thread should always be running, until finished

    /* Allocate space for Queues that store TCBs */
    for(int i = 0; i < NUM_THREAD_STATE; i++)
        tcb_queue[i] = queue_create();

    /* Enqueue Main TCB to Running Queue */
    queue_enqueue(tcb_queue[RUNNING_STATE], main_tcb);
} // init_main_thread()

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
}

int uthread_join(uthread_t tid, int *retval)
{
	/* Phase 2: Execute an infinite loop in which:
        If there are no more threads which are ready to run in the system, break the loop and return
        Otherwise simply yield to next available thread */

	while(1) {
	    /* Break when no more ready threads */
	    if(tcb_queue[READY_STATE].head == NULL) {
            break;
	    } // If: No more runnable/ready threads

	    /* Otherwise: Yied to next ready thread */
	    uthread_yield();
	}


	/* TODO Phase 3 */
} // uthread_join()

