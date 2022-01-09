#ifndef __TREE__
#define __TREE__

#include "utils.h"
#define FIRST_CHILD_ID "1"
#define SECOND_CHILD_ID "2"
#define STARTING_LEN 1
#define BUFFER_LEN 1000
#define CHILD_NOT_FOUND -1
#define PURGE_SUCCESS 0

/*Function used to alloc memory for a structure
 of type TTreeNode
 @returns: pointer to the newly allocated object*/
TTreeNode *createEmptyNode(void);

/*Function used to add a child to a specified parent node
 @params: parent:->parent node
          child:->child node*/
void addChild(TTree parent, TTree child, int set_id);

/*Wrapper over info's catContent used to concat.
 the contents of a string to TInfo's content field
@params: info:->info for which the concat. is done
         character:->string which will be concatenated*/
void addContent(TTree node, char *character);

/*Wrapper over info's catType used to concat.
 the contents of a string to TInfo's type field
@params: info:->info for which the concat. is done
         character:->string which will be concatenated*/
void addType(TTree node, char *character);

/*Function used to free the memory occupied by
 structure of type TTree(safe delete)
 @params: root->pointer to the root of the tree*/
void purgeTree(TTree *root);

/*Handler for generic stack deletion. Calls
 @purgeTree*/
void purgeTreeHandler(void *info);


/*Function used to set the ID of a subtree
 @params: root:->root of the subtree
          len:->integer holding the sibling number
          id:->string containing the starting id*/
void setSubtreeID(TTree root, int len, char *id);

/*Function used to set the ID of a tree
 @params: root:->root of the tree
*/
void setTreeID(TTree root);

/*Function used to append a sibling to a sibling list
 @params: node:->address of the sibling list
          sibling:->the node which will be appended
 to the list
 */
void appendSibling(TTree *node, TTree sibling);

/*Function used to remove a child from a child list
 (Doesn't free the memory occupied by child)
 @params: parent:->parent node for which the removal
 is done
          child:->child which will be removed from the
 list
 @returns: CHILD_NOT_FOUND if the child was not found
 in the child list or PURGE_SUCCESS if the purge
 was successful*/
int purgeChild(TTree parent, TTree child);

/*Function used to recursively delete all the nodes in a
 queue provided by a selector
 @params: root:->root of the tree
          selector_result:->queue containing the nodes
 obtained by applying a selector*/
void recursiveDelete(TTree root, TQueue *selector_result);

/*Function used to purge children in batch mode
 @params: node_queue:->queue containing all the child
 nodes which will be purged
          parent_node:->parent node for which the children
 are purged*/
void batchChildPurge(TQueue *node_queue, TTree parent_node);


#endif
