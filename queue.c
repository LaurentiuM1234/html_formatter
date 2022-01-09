/*Mihalcea Laurentiu 313CB*/
#include "queue.h"


TQueue *createQueue(size_t el_size)
{
    TQueue *queue = calloc(1, sizeof(TQueue));
    
    if (!queue)
        return NULL;
    
    queue->_el_size = el_size;
    
    return queue;
}


int enqueue(TQueue *queue, void *info)
{
    TNode *aux_node = calloc(1, sizeof(TNode));
    
    if (!aux_node)
        return ALLOC_FAIL;
    
    aux_node->_info = info;
    
    if (QUEUE_EMPTY(queue)) {
        queue->_first = aux_node;
        queue->_last = aux_node;
    } else {
        queue->_last->_next = aux_node;
        queue->_last = aux_node;
    }
    return ENQ_SUCCESS;
}


void *dequeue(TQueue *queue)
{
    if (QUEUE_EMPTY(queue))
        return NULL;
    
    void *info = queue->_first->_info;
    TNode *aux_node = queue->_first;
    
    if (queue->_first == queue->_last) {
        queue->_first = NULL;
        queue->_last = NULL;
    } else {
        queue->_first = queue->_first->_next;
    }
    
    free(aux_node);
    
    return info;
}


void *peek(TQueue *queue)
{
    if (QUEUE_EMPTY(queue))
        return NULL;
    
    return queue->_first->_info;
}


void moveFirst(TQueue *src, TQueue *dest)
{
    enqueue(dest, dequeue(src));
}

void printQueue(TQueue *queue, t_print_func print_queue,
                t_delete_handler delete_handler, FILE *fd)
{
    if (QUEUE_EMPTY(queue))
        return;
    else {
        TQueue *aux_queue = createQueue(sizeof(queue->_el_size));
        
        if (!aux_queue)
            return;
        
        while (!QUEUE_EMPTY(queue)) {
            void *aux_info = dequeue(queue);
            (*print_queue)(aux_info, fd);
            enqueue(aux_queue, aux_info);
        }
        
        while (!QUEUE_EMPTY(aux_queue))
            moveFirst(aux_queue, queue);
        
        deleteQueue(&aux_queue, delete_handler);
    }
}

void clearQueue(TQueue *queue, t_delete_handler delete_handler)
{
    //if no handler is provided use default delete
    if (!delete_handler) {
        while (!QUEUE_EMPTY(queue))
            free(dequeue(queue));
    } else {
        while (!QUEUE_EMPTY(queue))
            (*delete_handler)(dequeue(queue));
    }
}

void deleteQueue(TQueue **queue, t_delete_handler delete_handler)
{
    clearQueue(*queue, delete_handler);
    free(*queue);
    *queue = NULL;
}

