#ifndef __GEN_STACK__
#define __GEN_STACK__

#include "utils.h"

#define STACK_EMPTY(stack)((stack)->_top == NULL)
#define ALLOC_FAIL -1
#define PUSH_SUCCESS 0


/*Function used to allocate an element of type TStack
@params: el_size->size of the elements in the stack
@returns: NULL if allocation failed or the newly allocated
structure.
*/
TStack *createStack(size_t el_size);

/*Function used to push the info provided as
param into a stack
@params: stack->stack in which the element will
be pushed
         info->the information which will be pushed
in the stack
@returns: ALLOC_FAIL if node allocation failed or PUSH_SUCCESS
*/
int push(TStack *stack, void *info);


/*Function used to return top's info
 @returns: NULL if the stack is empty or a
 pointer to top's information*/
void *getTop(TStack *stack);

/*Function used to retrieve the top of a stack
 @params: stack->stack from which the top is extracted
 @returns: a pointer to the information */
void *pop(TStack *stack);

/*Function used to move the top of one stack
to another stack
@params: src->source stack
         dest->destination stack*/
void moveTop(TStack *src, TStack *dest);


/*Function used to print the contents of a stack using
 a function provided by the user
 @params: print_stack->pointer to the function which will
 be used for printing
 */
void printStack(TStack *stack, t_print_func print_stack,
                t_delete_handler delete_handler, FILE *fd);

/*Function used to delete a stack(safe delete).*/
void deleteStack(TStack **stack, t_delete_handler delete_handler);


/*Function used to free the contents of a stack.*/
void clearStack(TStack *stack, t_delete_handler delete_handler);


#endif
