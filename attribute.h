#ifndef __ATTRIBUTE__
#define __ATTRIBUTE__

#include "utils.h"

#define NAME_LENGTH 1000
#define VALUE_LENGTH 1000
#define ATTRIBUTE_NOT_FOUND -1
#define ATTRIBUTE_FOUND 0
#define STYLE_SPLITTER ";"
#define SUBATTR_SPLITTER ":"

/*Function used to alloc memory for a structure of type
 TAttrNode
 @returns: pointer to newly allocated object*/
TAttrNode *allocNode(void);

/*Function used to fill out TAttrNode's name field
 @params: node:->pointer to the node for which the
 filling is done
          name:->string containing the name which will
 be used to fill the structure's field*/
void setName(TAttrNode *node, char *name);

/*Function used to fill out TAttrNode's value field
@params: node:->pointer to the node for which the
filling is done
         value:->string containing the value which will
be used to fill the structure's field*/
void setValue(TAttrNode *node, char *value);

/*Function used to create a node(allocation is implicit)
 @params: name:->string containing the name of the
 attribute
          value:->string containing the value of the
 attribute*/
TAttrNode *createNode(char *name, char *value);

/*Function used to append a node to a list
 @params: list->pointer to a list in which the
 appending operation will be done
          node:->node which will be appended to
 the list*/
void append(TAttrList *list, TAttrNode *node);

/*Function used to free the memory occupied by structure
 of type TAttrList(safe delete)
 @params: list->address of the list which will be freed*/
void deleteList(TAttrList *list);

/*Function used to free the memory occupied by structure
of type TAttrNode
@params: node->node which will be freed*/
void deleteNode(TAttrNode *node);

/*Function used to split a style string into multiple
 attributes; the function also combines the attributes
 into an attribute list
 @params: style_string:->string containing the style
 @returns: an attribute list or NULL if any of the
 allocations failed*/
TAttrList splitStyleString(char *style_string);

/*Function used to print an attribute list
 @params: list->the attribute list
          output_file:->pointer to an already opened
 output file
          is_style:->flag(0/1) which tells the function
 to print the list in a certain way*/
void printList(TAttrList list, FILE *output_file, int is_style);

/*Function used to search for an attribute
 name and value in an attribute list
 @params: list:->the attribute list
          attribute_name:->string containing the name
 of the attribute(mandatory)
          attribute_value:->string containing the value
 of the attribute(optional)*/
int searchAttribute(TAttrList list, char *attribute_name,
                    char *attribute_value);

/*Function used to override the style of the nodes
 obtained by applying a selector
 @params: selector_result:->queue containing the nodes
 obtained by applying a selector
          style_string:->string containing the new style*/
void overrideStyle(TQueue *selector_result, char *style_string);

/*Function used to append to the style of the nodes
obtained by applying a selector
@params: selector_result:->queue containing the nodes
obtained by applying a selector
         style_string:->string containing the new style*/
void appendStyle(TQueue *selector_result, char *style_string);

/*Function used to update the values of common style
 names between a node and a list
 @params: list:->the style list
          node:->node which will be used to update the
 values from the style list if it has the same style
 names as the node*/
void update(TAttrList *list, TAttrNode *node);


#endif
