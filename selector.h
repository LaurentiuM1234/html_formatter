#ifndef __SELECTORS__
#define __SELECTORS__

#include "utils.h"


/*Function implementing the CSS ID selector
 @params: root:->root of the tree
          id:->string containing the id
          output_queue:->queue containing the
 node of interest(will be unallocated if the node is
 not found
          id_match:->string containing the digit
 number in id string which have to match*/
void IDSelect(TTree root, char *id, TQueue *output_queue, int id_match);

/*Function implementing the CSS class selector
 @params: root:->root of the tree
          class_name:->string containing the
 name of the class
 @returns: a queue containing the tree nodes of
 interest*/
TQueue *classSelect(TTree root, char *class_name);

/*Function implementing the CSS element selector
@params: root:->root of the tree
         tag_name:->string containing the
name of the tag
@returns: a queue containing the tree nodes of
interest*/
TQueue *elementSelector(TTree root, char *tag_name);

/*Function implementing the CSS element.class selector
@params: root:->root of the tree
         tag_name:->string containing the
name of the tag
         class_name:->string containing the
name of the class
@returns: a queue containing the tree nodes of
interest*/
TQueue *ecSelector(TTree root, char *tag_name, char *class_name);

/*Function implementing the CSS element1 element2 selector
@params: root:->root of the tree
         parent_tag:->string containing the
name of the parent tag
         class_name:->string containing the
name of the child_tag
@returns: a queue containing the tree nodes of
interest*/
TQueue *eeSelector(TTree root, char *parent_tag, char *child_tag);

/*Function implementing the CSS element1>element2 selector
@params: root:->root of the tree
         parent_tag:->string containing the
name of the parent tag
         class_name:->string containing the
name of the child_tag
@returns: a queue containing the tree nodes of
interest*/
TQueue *epeSelector(TTree root, char *parent_tag, char *child_tag);


#endif
