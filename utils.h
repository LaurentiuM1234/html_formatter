#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef void (*t_print_func)(void *, FILE *);
typedef void (*t_delete_handler)(void *);

typedef struct node
{
    void *_info;
    struct node *_next;
    
}TNode, *TList;

typedef struct
{
    size_t _el_size;
    TList _top;
    
}TStack;

typedef struct Attribute
{
    char *_name;
    char *_value;
    struct Attribute *_next;
    
}TAttrNode, *TAttrList;

typedef struct Info
{
    char *_type;
    char *_id;
    TAttrList _style;
    TAttrList _other_attributes;
    int _is_self_closing;
    char *_contents;
    
} *TInfo;

typedef struct Node
{
    TInfo _info;
    struct Node *_next_sibling;
    struct Node *_first_child;
    
}TTreeNode, *TTree;

typedef struct
{
    size_t _el_size;
    TList _first, _last;
}TQueue;

typedef enum
{
    PARSE_ERROR             = 0,
    PARSE_CONTENTS          = 1,
    PARSE_OPENING_BRACKET   = 2,
    PARSE_TAG_TYPE          = 3,
    PARSE_CLOSING_TAG       = 4,
    PARSE_REST_OF_TAG       = 5,
    PARSE_ATTRIBUTE_NAME    = 6,
    PARSE_ATTRIBUTE_EQ      = 7,
    PARSE_ATTRIBUTE_VALUE   = 8,
    PARSE_SELF_CLOSING      = 9,
} TParseState;

TParseState Interpret(TParseState currentState, char c);


#endif
