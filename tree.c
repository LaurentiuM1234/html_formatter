/*Mihalcea Laurentiu 313CB*/
#include "tree.h"
#include "info.h"
#include "queue.h"


TTreeNode *createEmptyNode(void)
{
    TTreeNode *node = calloc(1, sizeof(TTreeNode));
    
    if (!node)
        return NULL;
    
    node->_info = createInfo();
    
    if (!node->_info) {
        free(node);
        return NULL;
    }
    
    return node;
}

void addChild(TTree parent, TTree child, int set_id)
{
    if (!set_id) {
        if (!parent->_first_child)
            parent->_first_child = child;
        else {
            TTree aux_node = parent->_first_child;
            
            while (aux_node->_next_sibling != NULL)
                aux_node = aux_node->_next_sibling;
            
            aux_node->_next_sibling = child;
        }
    } else {
        if (!parent->_first_child) {
            parent->_first_child = child;
            sprintf(child->_info->_id, "%s.%s",
                    parent->_info->_id, "1");
        }
        else {
            TTree aux_node = parent->_first_child;
            int order = 1;
            
            while (aux_node->_next_sibling != NULL) {
                aux_node = aux_node->_next_sibling;
                order++;
            }
            
            aux_node->_next_sibling = child;
            sprintf(child->_info->_id, "%s.%d",
                    parent->_info->_id, order + 1);
        }
    }
}

void addContent(TTree node, char *character)
{
    catContent(node->_info, character);
}

void addType(TTree node, char *character)
{
    catType(node->_info, character);
}

void purgeTree(TTree *root)
{
   if (!(*root))
       return;
    purgeTree(&(*root)->_next_sibling);
    purgeTree(&(*root)->_first_child);
    deleteInfo(&(*root)->_info);
    free(*root);
    *root = NULL;
}

void purgeTreeHandler(void *info)
{
    TTree root = (TTree)info;
    
    purgeTree(&root);
}


void setSubtreeID(TTree root, int len, char *id)
{
    if (!root)
        return;
    
    char buffer[BUFFER_LEN];
    
    sprintf(buffer, "%s.%d", id, len);
    
    setID(root->_info, buffer);
    
    setSubtreeID(root->_first_child, STARTING_LEN, root->_info->_id);
    setSubtreeID(root->_next_sibling, len + 1, id);
    
}

void setTreeID(TTree root)
{
    TTree first_child = root->_first_child;
    TTree second_child = root->_first_child->_next_sibling;
    
    if (first_child) {
        setID(first_child->_info, FIRST_CHILD_ID);
        setSubtreeID(first_child->_first_child,
                     STARTING_LEN, FIRST_CHILD_ID);
    }
    if (second_child) {
        setID(second_child->_info, SECOND_CHILD_ID);
        setSubtreeID(second_child->_first_child,
                     STARTING_LEN, SECOND_CHILD_ID);
    }
}

void appendSibling(TTree *node, TTree sibling)
{
    if (!(*node))
        *node = sibling;
    else {
        TTree aux_node = *node;
        
        while (aux_node->_next_sibling != NULL)
            aux_node = aux_node->_next_sibling;
        
        aux_node->_next_sibling = sibling;
    }
}

int purgeChild(TTree parent, TTree child)
{
    if (parent->_first_child == child) {
        parent->_first_child = child->_next_sibling;
    } else {
        TTree aux_node = parent->_first_child;
        
        while (aux_node) {
            if (aux_node->_next_sibling == child)
                break;
            aux_node = aux_node->_next_sibling;
        }
        if (!aux_node)
            return CHILD_NOT_FOUND;
        aux_node->_next_sibling = child->_next_sibling;
    }
    child->_next_sibling = NULL;
    return PURGE_SUCCESS;
}

void recursiveDelete(TTree root, TQueue *selector_result)
{
    if (!root)
        return;
    
    if (QUEUE_EMPTY(selector_result))
        return;
    
    batchChildPurge(selector_result, root);
    
    recursiveDelete(root->_first_child, selector_result);
    recursiveDelete(root->_next_sibling, selector_result);
}

void batchChildPurge(TQueue *node_queue, TTree parent_node)
{
    TQueue *aux_queue = createQueue(sizeof(TTree));
    
    if (!aux_queue)
        return;
    
    while (!QUEUE_EMPTY(node_queue)) {
        TTree child_node = dequeue(node_queue);
        
        if (purgeChild(parent_node, child_node) == PURGE_SUCCESS)
            purgeTree(&child_node);
        else
            enqueue(aux_queue, child_node);
        
    }
    while (!QUEUE_EMPTY(aux_queue))
        moveFirst(aux_queue, node_queue);
    
    deleteQueue(&aux_queue, NULL);
}
