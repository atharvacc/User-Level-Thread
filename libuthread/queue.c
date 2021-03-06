#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

/* Choosing Linked List over Array due to enqueue() and dequeue() being O(1)
 as required by the assignment. (Array's enqueue() and dequeue() is Armortized O(1) */
struct Node {
    void *data;
    struct Node* next;
};

struct queue {
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
	if(queue == NULL || queue->head != NULL) {
	    return -1;
	} // If: Queue is NULL, or Queue is not empty

	free(queue);
	return 0;
} // queue_destroy() -- Tested

int queue_enqueue(queue_t queue, void *data)
{
    if(queue == NULL || data == NULL) {
        return -1;
    } // If: Queue or Data is NULL

	if(queue->head == NULL) {
	    queue->head = (struct Node*)malloc(sizeof(struct Node));
	    queue->head->data = data;
	    queue->back = queue->head;
		queue->head->next = NULL;
	} // If: Empty Queue
	else {
	    queue->back->next = (struct Node*)malloc(sizeof(struct Node));
	    queue->back->next->data = data;
	    queue->back = queue->back->next;
		queue->back->next = NULL;
		
	} // Else: Existing Queue

	if(queue->head == NULL) {
	    return -1;
	} // If: Failed to Malloc

	return 0;
} // queue_enqueue() -- Tested

int queue_dequeue(queue_t queue, void **data)
{
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

    struct Node *current = NULL;
	struct Node *prev = NULL;
	struct Node *deleteNode = NULL;

	for(current = queue->head; current != queue->back; current = current->next) {
	    if(current->next->data == data)
	        prev = current;
	} // Loop through Queue

	if(prev == NULL) {
        if(queue->head->data == data) {
            deleteNode = queue->head;
            queue->head = queue->head->next;
            free(deleteNode);
        } // If: First element holds data
        else {
            return -1;
        } // Else: Data not found
	} // If: Found in Queue
	else {
        deleteNode = prev->next;
        if(prev->next == queue->back) {
            prev->next = NULL;
            queue->back = prev;
        } // Edge Case: delete last element
        else {
            prev->next = prev->next->next;
        } // Else: normal delete

        free(deleteNode);
	} // Else: Found in Queue

	return 0;
} // queue_delete -- Tested
/*
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
    if(queue == NULL || func == NULL)
        return -1;
	struct Node *current = NULL;
	for(current = queue->head; current != queue->back; current = current->next) {
	    if(func(current->data, arg) == 1) {
	        if(current->data != NULL)
	            data = current->data;
	        break;
	    } // If: @func returns 1. Stop prematurely
	} // Loop: through Queue
	// Account for last element
	struct Node *lastElement = queue->back;
    if(func(lastElement->data, arg) == 1)
        if(lastElement->data != NULL)
            data = lastElement->data;
	return 0;
}*/
int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func ==NULL){
		return -1;
	}
	for(struct Node *current = queue->head; current != queue->back; current = current->next) {

		if(func(current->data, arg)){
			if(data!=NULL){
				*data = current->data;
			}
			return 0;
		}
    } //end for

	//Do the same for the last element
	if(func(queue->back->data, arg)){
			if(data!=NULL){
				*data = queue->back->data;
			}
			return 0;
		}
    return 0;
}

int queue_length(queue_t queue)
{
    int count = 0;

	if(queue == NULL)
	    return -1;
	if(queue->head == NULL)
	    return 0;

    for(struct Node *current = queue->head; current != queue->back; current = current->next)
        count += 1;

	return count + 1; // To take into account the last element
} // Has error. for empty queue doesn't wokr as expected

int print_queue(queue_t queue) {
    printf("Printing Queue:\n");
    for(struct Node *current = queue->head; current != queue->back; current = current->next) {
        printf("%d ", *(int*)current->data);
    }
    printf("%d", *(int*)queue->back->data);
    printf("\n--------------------------\n\n");

    return 0;
}

