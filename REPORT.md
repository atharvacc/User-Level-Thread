### Queue Implementation 

For this part, we have chosen Linked List as the data structure for the Queue over an Array.
The reason is that the LinkedList provide an O(1) time for 'enqueue' and 'dequeue' operations,
as requested by the prompt; while an Array provide an O(1) amortized time for 'enqueue' and 'dequeue'.

#### queue_create
For this function, we use malloc to allocate space for the queue, and add a check to see whether malloc
was successfully executed or not

#### queue_destroy
For this function, we use 'free' to deallocate space of the queue. Add check for when queue is NULL
or queue is empty

#### queue_enqueue
For this function, we first check that the Queue is not NULL, and the Data to enqueue is not NULL. Then,
if it's an empty queue, we allocate space for a Linked List Node in the Queue, saves the data there, and
set that node as the head of the Queue. If the queue is not empty, we also allocate space for a Node, and
add that Node to the back of the queue, which can be accessed by 'queue->back->next'. Update the back node.

#### queue_dequeue 
For this function, we check for Queue is NULL/Queue is Empty/Data is NULL. We saved the data from the Node we
are dequeueing to 'data'. We then free that Node (first node of Queue), and then update the head to head->next.

#### queue_delete
For this function, we first loop over to find the Node previous to the Node that holds the data we want. 
We do this because we want to be able to update the previous Node's next to the Node after the Node we want; or 
else we can't keep the list linked. We add edge cases for when deleting the first or last element of the queue; 
that is because for those two operations, we need to update the Queue's head and back.

#### queue_iterate
For this function, we used the call back function 'func' to update the data of the queue.
What to be noted is that those functions should not be able to manipulate the queue, just the data. We check
for the function return value; if it is 1, we would stop prematurely, and not updating the elements after that.
In addition, if the pointer to data list is not NULL, we would save the node's data to that data list.

#### queue_length
For this function, we basically loop over the loop, and increment the count as we go.

#### Testing
To test the functions of the Queue, we wrote a test for each function to check whether they are working
properly or not, including the edge cases.

_test_create():_ Check whether the Queue is NULL or not (if it's not NULL, we have allocated space)

_test_queue_simple():_ Check whether enqueue and dequeue work or not

_test_destroy():_ Check whether we are calling free on valid Queues (Queue != NULL, and Queue is empty) or not

_test_delete():_ Check whether we have deleted a certain element from the queue or not

_test_length():_ Check whether the length is correct or not (Empty, with some elements, empty due to 'delete'/'dequeue'

_test_iterate():_ Check whether we are changing the data inside the queue or not. Stop prematurely if func's retval = 1

### UThread Implementation

For this part, we used a struct called TCB that stores the following:
- TID: thread identifier
- context: context for the program
- stack: stack needed for the thread
- retVal: On exiting, the return value is assigned to this
- isBlocked: Checks if the thread is Blocked. 
- *retValSave: pointer where we want to store the retval for a certain TID
- tidWait: If the thread is blocked, then this is the identifier for the TID of the blocked thread 

We also use 3 queues.
- READY: To have all of the READY threads
- BLOCKED: To have all of the BLOCKED threads
- ZOMBIE: To store all of the exited threads for collection later on.
#### uthread_create
This creates the main thread the first time it is called. It also initializes the READY queue when it is called for the first time.
It uses the context initializer to initialize the context for the new thread. Add enqueues it to the READY queue. This controls the 
global variable cur_tid which is used to assign a TID to new threads. The main thread is assigned a TID of 0.

#### uthread_self
This just returns the TID of the currently running thread.

#### uthread_exit
This forms an important aspect of our code. Once a thread is done, this is called within ctx_bootstrap. Initially, it will assign the thread to a ZOMBIE state.
Then it looks at the blocked queue to see if we had a thread waiting for it. If it did, it will unblock it and add it to the READY queue. Then it delete the 
exiting thread from the ZOMBIE queue, and frees up its context and deletes it. After which, this will do a context switch to the next thread available in the
READY queue.

#### uthread_yield
If the READY queue was never initialized or if nothing is in the READY queue then it will just return. Else it will save the current thread if the threads
isBlocked is not set to 1. And swap the oldest thread from the READY queue with the currently running thread.


#### uthread_join
If the BLOCKED queue is NULL, then it initializes it. If the thread we are waiting for has tid "0"( meaning we wait for the main thread), then we return a -1 
since if the main thread exits then the process is done or if the currently running thread is what we want to join to then it will return -1 as well. Then we check in the
ZOMBIE queue for the TID. If it is present, then we delete it from the ZOMBIE after assigning the retVal of the exited thread to the current thread. If *retval was provided then
we assign it to the retval provided. We then check for the TID in the READY and blocked queue. If present in ready we update the BLOCKED QUEUE with the cur_thread. Assign the 
isBlocked to 1, and then yield. Which will schedule the next available thread, and not enqueue the blocked thread to the ready queue. If it is present in the BLOCKED queue, then we block the currently running thread, by enqueing it into the BLOCKED QUEUE and set the isBlocked value to 1. If none of these if/else statements has the value, then the TID is not present anywhere and we return a -1.

#### Testing 
- We ran uthread_hello to ensure our thread is performing as expected.
- We also ran uthread_yield to ensure our thread is performing as expected.
- We made a modification to uthread_hello to check if we assign an int pointer to store the return value, it is actually assigned to it after joining.
- We added a yield statement without creating a thread to check if we return without our code crashing.

### Preempt Implementation and usage within uthread
- We have a signal handler that just calls uthread_yield after the timer goes off.
- For disabling, we define an empty sigset_t  and the the SIGVTALRM signal to it. We use sigprocmask to disable and enable the preempts.

##### Usage within uthread.c
- uthread_yield: We disable the preempt when do the swap between the currently executing thread, and the next one. If we don't then its possible that after enqueing the READY queue, 
we get an interrupt and start the process all over again, causing the READY queue to be updated twice.
- uthread_create: When we initialize the main thread, thats when we start the preemption. However, when creating the new thread we do disable it since we want the cur_tid to be updated atomically, and the
TID of the current thread as well.
- uthread_exit: We disable the preempt before enqueing the current thread in the ZOMBIE list, and proceed to do unblocking. This is important, since unblocking modifies whats in the READY queue. 
- uthread_join: We have different if statements, that check values within the data structure. We do not want to be yielding to a different thread and updating the queues while in join. Thus we disable it before checking these if statements and enable it after

