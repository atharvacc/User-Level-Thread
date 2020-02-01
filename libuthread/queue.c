#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Choosing Linked List over Array due to enqueue() and dequeue() being O(1)
 as required by the assignment. (Array's enqueue() and dequeue() is Armortized O(1) */
struct Node {
    void *data;
    struct Node* next;
};

struct queue {
	/* TODO Phase 1 */
	struct Node* head;
	struct Node* back;
};

queue_t queue_create(void)
{
    struct queue* newQueue = NULL;
    newQueue = (struct queue*)malloc(sizeof(struct queue));
    if(newQueue != NULL) {
        newQueue->head = NULL;
        newQueue->back = NULL;
    } // If: newQueue is malloc-ed successfully
    else {
        return NULL;
    } // Else: newQueue failed to be allocated

    return newQueue;
} // queue_create() -- Tested

int queue_destroy(queue_t queue)
{
	/* TODO Phase 1 */
	if(queue == NULL || queue->head != NULL) {
	    return -1;
	} // If: Queue is NULL, or Queue is not empty

	free(queue);
	return 0;
} // queue_destroy()

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data == NULL) {
	    return -1;
	} // If: Queue or Data is NULL

	if(queue->head == NULL) {
	    queue->head = (struct Node*)malloc(sizeof(struct Node));
	    queue->head->data = data;
	    queue->back = queue->head;
	} // If: Empty Queue
	else {
	    queue->back->next = (struct Node*)malloc(sizeof(struct Node));
	    queue->back->next->data = data;
	    queue->back = queue->back->next;
	} // Else: Existing Queue

	return 0;
} // queue_enqueue() -- Tested

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if(queue == NULL || data == NULL || queue->head == NULL) {
	    return -1;
	} // If: Queue/Data is NULL, or if Queue is Empty

	*data = queue->head->data;
	struct Node* temp = queue->head->next;
	free(queue->head);
	queue->head = temp;

	return 0;
} // queue_dequeue() -- Tested

int queue_delete(queue_t queue, void *data)
{
	if(queue == NULL || data == NULL) {
        return -1;
    } // If: Queue is NULL or Data is NULL

	struct Node *findNode = queue->head;

	while(findNode->next != NULL && findNode->next->data != data) {
        findNode = findNode->next;
	} // Find the Node which holds the data to delete

	if(findNode->next == NULL) {
	    return -1;
	} // If: Data not found TODO: What if last
	else {
	    struct Node *deleteNode = findNode->next;
	    findNode->next = findNode->next->next;
	    free(deleteNode);
	} // Else: Data is found

	return 0;
} // queue_delete -- Tested

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

