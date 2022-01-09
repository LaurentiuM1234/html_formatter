#ifndef __GEN_QUEUE__
#define __GEN_QUEUE__


#include "utils.h"


#define QUEUE_EMPTY(queue)((queue)->_first == NULL && (queue)->_last == NULL)
#define ALLOC_FAIL -1
#define ENQ_SUCCESS 0

/*Function used to allocate an element of type TQueue
@params: el_size->size of the elements in the queue
@returns: NULL if allocation failed or the newly allocated
structure.
*/
TQueue *createQueue(size_t el_size);

/*Function used to enqueue the info provided as
param into a queue
@params: queue->queue in which the element will
be pushed
         info->the information which will be pushed
in the queue
@returns: ALLOC_FAIL if node allocation failed
or ENQ_SUCCESS
*/
int enqueue(TQueue *queue, void *info);

/*Function used to retrieve the first element
 of the queue
 @params: queue->queue from which the first
 element is extracted*/
void *dequeue(TQueue *queue);

/*Function used to return first's info
@returns: NULL if the queue is empty or a
pointer to first's information*/
void *peek(TQueue *queue);

/*Function used to move the first element
of one queue to another queue
@params: src->source queue
         dest->destination queue
 */
void moveFirst(TQueue *src, TQueue *dest);

/*Function used to print the contents of a queue using
a function provided by the user
@params: print_queue->pointer to the function which will
be used for printing
         delete_handler->pointer to the function which
 will be used to delete the auxiliary queue used
*/
void printQueue(TQueue *queue, t_print_func print_queue,
                t_delete_handler delete_handler, FILE *fd);

/*Function used to delete a queue(safe delete).*/
void deleteQueue(TQueue **queue, t_delete_handler delete_handler);

/*Function used to delete the elements of a queue.*/
void clearQueue(TQueue *queue, t_delete_handler delete_handler);

#endif

