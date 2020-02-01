#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)			\
do {						\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {				\
		printf("PASS\n");		\
	} else	{				\
		printf("FAIL\n");		\
		exit(1);			\
	}					\
} while(0)

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Destroy */
void test_destroy(void) {
    int data = 3;
    fprintf(stderr, "*** TEST destroy ***\n");

    // Check: Return -1 if @queue is NULL
    queue_t newQueue= NULL;
    TEST_ASSERT(queue_destroy(newQueue) == -1);
    // Check: Return -1 if @queue is not empty
    newQueue = queue_create();
    queue_enqueue(newQueue, &data);
    TEST_ASSERT(queue_destroy(newQueue) == -1);
    // Check: Queue is successfully destroyed
    newQueue = queue_create();
    TEST_ASSERT(queue_destroy(newQueue) == 0);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &data);
}

/* Delete Queue */
void test_delete(void) {
    int dataArray[10] = {1,2,3,4,5,6,7,8,9,10};
    queue_t newQueue = queue_create();

    for(int i = 0; i < sizeof(dataArray); i++) {
        queue_enqueue(newQueue, &dataArray[i]);
    } // Enqueue 1-10

    fprintf(stderr, "*** TEST queue_delete ***\n");

    queue_delete(newQueue, &dataArray[0]); // Delete '1'
    TEST_ASSERT(queue_delete(newQueue, &dataArray[0]) != 0); // '1' should not be found

    queue_delete(newQueue, &dataArray[4]); // Delete '5'
    TEST_ASSERT(queue_delete(newQueue, &dataArray[4]) != 0); // '5' should not be found

    queue_delete(newQueue, &dataArray[9]); // Delete '10'
    TEST_ASSERT(queue_delete(newQueue, &dataArray[9]) != 0); // '10' should not be found
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_destroy();
    test_delete();

	return 0;
}
