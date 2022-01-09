/*Mihalcea Laurentiu 313CB*/
#include "selector.h"
#include "attribute.h"
#include "tree.h"
#include "queue.h"

void IDSelect(TTree root, char *id, TQueue *output_queue, int id_match)
{
    if (!root)
        return;
    
    if (id[id_match] == root->_info->_id[id_match]) {
        if (strcmp(root->_info->_id, id) == 0)
               enqueue(output_queue, root);
        else
            IDSelect(root->_first_child, id, output_queue, id_match + 2);
    } else
        IDSelect(root->_next_sibling, id, output_queue, id_match);
    
}

TQueue *classSelect(TTree root, char *class_name)
{
    TQueue *output_queue = createQueue(sizeof(TTree));
    
    if (!output_queue)
        return NULL;
    TQueue *iteration_queue = createQueue(sizeof(TTree));
    
    if (!iteration_queue) {
        deleteQueue(&output_queue, NULL);
        return NULL;
    }
    
    enqueue(iteration_queue, root);
    
    while (!QUEUE_EMPTY(iteration_queue)) {
        TTree current_root = dequeue(iteration_queue);
        
        if (searchAttribute(current_root->_info->_other_attributes,
                            "class", class_name) == ATTRIBUTE_FOUND)
            enqueue(output_queue, current_root);
        
        TTree aux_node = current_root->_first_child;
        
        while (aux_node) {
            enqueue(iteration_queue, aux_node);
            aux_node = aux_node->_next_sibling;
        }
    }
    deleteQueue(&iteration_queue, NULL);
    return output_queue;
}

TQueue *elementSelector(TTree root, char *tag_name)
{
    TQueue *output_queue = createQueue(sizeof(TTree));
    
    if (!output_queue)
        return NULL;
    TQueue *iteration_queue = createQueue(sizeof(TTree));
    
    if (!iteration_queue) {
        deleteQueue(&output_queue, NULL);
        return NULL;
    }
    
    enqueue(iteration_queue, root);
    
    while (!QUEUE_EMPTY(iteration_queue)) {
        TTree current_root = dequeue(iteration_queue);
        
        if (strcmp(current_root->_info->_type, tag_name) == 0)
            enqueue(output_queue, current_root);
        
        TTree aux_node = current_root->_first_child;
        
        while (aux_node) {
            enqueue(iteration_queue, aux_node);
            aux_node = aux_node->_next_sibling;
        }
    }
    deleteQueue(&iteration_queue, NULL);
    
    return output_queue;
}

TQueue *ecSelector(TTree root, char *tag_name, char *class_name)
{
    TQueue *output_queue = createQueue(sizeof(TTree));
    
    if (!output_queue)
        return NULL;
    TQueue *iteration_queue = createQueue(sizeof(TTree));
    
    if (!iteration_queue) {
        deleteQueue(&output_queue, NULL);
        return NULL;
    }
    
    enqueue(iteration_queue, root);
    
    while (!QUEUE_EMPTY(iteration_queue)) {
        TTree current_root = dequeue(iteration_queue);
        
        if (strcmp(current_root->_info->_type, tag_name) == 0 &&
            searchAttribute(current_root->_info->_other_attributes, "class",
                            class_name) == ATTRIBUTE_FOUND)
            enqueue(output_queue, current_root);
        
        TTree aux_node = current_root->_first_child;
        
        while (aux_node) {
            enqueue(iteration_queue, aux_node);
            aux_node = aux_node->_next_sibling;
        }
    }
    deleteQueue(&iteration_queue, NULL);
    
    return output_queue;
    
}


TQueue *eeSelector(TTree root, char *parent_tag, char *child_tag)
{
    TQueue *output_queue = createQueue(sizeof(TTree));
    
    if (!output_queue)
        return NULL;
    
    TQueue *iteration_queue = createQueue(sizeof(TTree));
    
    if (!iteration_queue) {
        deleteQueue(&output_queue, NULL);
        return NULL;
    }
    
    TQueue *input_queue = elementSelector(root, parent_tag);
    
    if (!input_queue) {
        deleteQueue(&output_queue, NULL);
        deleteQueue(&iteration_queue, NULL);
        return NULL;
    }
    
    while (!QUEUE_EMPTY(input_queue)) {
        enqueue(iteration_queue, dequeue(input_queue));
        
        while (!QUEUE_EMPTY(iteration_queue)) {
            
            TTree current_root = dequeue(iteration_queue);
            
            if (strcmp(current_root->_info->_type, child_tag) == 0)
                enqueue(output_queue, current_root);
            
            TTree aux_node = current_root->_first_child;
            
            while (aux_node) {
                enqueue(iteration_queue, aux_node);
                aux_node = aux_node->_next_sibling;
            }
        }
    }
    
    deleteQueue(&input_queue, NULL);
    deleteQueue(&iteration_queue, NULL);
    return output_queue;
}

TQueue *epeSelector(TTree root, char *parent_tag, char *child_tag)
{
    TQueue *output_queue = createQueue(sizeof(TTree));
    
    if (!output_queue)
        return NULL;
    
    TQueue *iteration_queue = createQueue(sizeof(TTree));
    
    if (!iteration_queue) {
        deleteQueue(&output_queue, NULL);
        return NULL;
    }
    
    TQueue *input_queue = elementSelector(root, parent_tag);
    
    if (!input_queue) {
        deleteQueue(&output_queue, NULL);
        deleteQueue(&iteration_queue, NULL);
        return NULL;
    }
    
    while (!QUEUE_EMPTY(input_queue)) {
        TTree current_root = dequeue(input_queue);
        
        TTree aux_node = current_root->_first_child;
        
        while (aux_node) {
            enqueue(iteration_queue, aux_node);
            aux_node = aux_node->_next_sibling;
        }
    }
    while (!QUEUE_EMPTY(iteration_queue)) {
        TTree current_node = dequeue(iteration_queue);
        
        if (strcmp(current_node->_info->_type, child_tag) == 0)
            enqueue(output_queue, current_node);
    }
    
    deleteQueue(&input_queue, NULL);
    deleteQueue(&iteration_queue, NULL);
    return output_queue;
}
