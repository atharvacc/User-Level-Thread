### Queue Implementation 

( NEEDS WORK. IDK MUCH ABOUT THIS)

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


