/*Mihalcea Laurentiu 313CB*/
#include "stack.h"

TStack *createStack(size_t el_size)
{
    TStack *stack = calloc(1, sizeof(TStack));
    
    if (!stack)
        return NULL;
    
    stack->_el_size = el_size;
    
    return stack;
}

int push(TStack *stack, void *info)
{
    TNode *aux_node = calloc(1, sizeof(TNode));
    
    if (!aux_node)
        return ALLOC_FAIL;
    
    aux_node->_info = info;

    if (STACK_EMPTY(stack))
        stack->_top = aux_node;
    else {
        aux_node->_next = stack->_top;
        stack->_top = aux_node;
    }
    return PUSH_SUCCESS;
}

void *getTop(TStack *stack)
{
    if (STACK_EMPTY(stack))
        return NULL;
    else
        return stack->_top->_info;
}

void *pop(TStack *stack)
{
    if (STACK_EMPTY(stack))
        return NULL;
    
    void *info = stack->_top->_info;
    TNode *aux_node = stack->_top;
    stack->_top = stack->_top->_next;
    free(aux_node);
    
    return info;
}

void moveTop(TStack *src, TStack *dest)
{
    push(dest, pop(src));
}


void printStack(TStack *stack, t_print_func print_stack,
                t_delete_handler delete_handler, FILE *fd)
{
    if (STACK_EMPTY(stack))
        return;
    else {
        TStack *aux_stack = createStack(stack->_el_size);
        
        if (!aux_stack)
            return;
        
        while (!STACK_EMPTY(stack)) {
            void *cur_element = pop(stack);
            (*print_stack)(cur_element, fd);
            push(aux_stack, cur_element);
        }
        while (!STACK_EMPTY(aux_stack))
            moveTop(aux_stack, stack);
        
        deleteStack(&aux_stack, delete_handler);
    }
}

void deleteStack(TStack **stack, t_delete_handler delete_handler)
{
    clearStack(*stack, delete_handler);
    free(*stack);
    *stack = NULL;
}

void clearStack(TStack *stack, t_delete_handler delete_handler)
{
    if (!delete_handler) {
        //No delete handler is provided
        while (!STACK_EMPTY(stack))
            free(pop(stack));
    } else {
        while(!STACK_EMPTY(stack))
            (*delete_handler)(pop(stack));
    }
}

