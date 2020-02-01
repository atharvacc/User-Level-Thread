#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue {
	int count; // Get element count
	void *HEAD;
	void *TAIL; 
};


/*
 * queue_create - Allocate an empty queue
 *
 * Create a new object of type 'struct queue' and return its address.
 *
 * Return: Pointer to new empty queue. NULL in case of failure when allocating
 * the new queue.
 */
queue_t queue_create(void)
{
	struct queue newQueue = {NULL, NULL,NULL};
	return &newQueue;
	/* TODO Phase 1 */
}

/*
 * queue_destroy - Deallocate a queue
 * @queue: Queue to deallocate
 *
 * Deallocate the memory associated to the queue object pointed by @queue.
 *
 * Return: -1 if @queue is NULL or if @queue is not empty. 0 if @queue was
 * successfully destroyed.
 */
int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->count != 0){
		return -1;
	}
	free(queue);
	return 0;
}

/*
 * queue_enqueue - Enqueue data item
 * @queue: Queue in which to enqueue item
 * @data: Address of data item to enqueue
 *
 * Enqueue the address contained in @data in the queue @queue.
 *
 * Return: -1 if @queue or @data are NULL, or in case of memory allocation error
 * when enqueing. 0 if @data was successfully enqueued in @queue.
 */
int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

int main()
{
    queue_t newqueue = queue_create();
	return 0;
}