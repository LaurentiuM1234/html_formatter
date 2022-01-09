/*Mihalcea Laurentiu 313CB*/
#include "attribute.h"
#include "queue.h"
#include "fop.h"


TAttrNode *allocNode(void)
{
    TAttrNode *node = calloc(1, sizeof(TAttrNode));
    
    if (!node)
        return NULL;
    
    node->_name = calloc(NAME_LENGTH, sizeof(char));
    
    if (!node->_name) {
        free(node);
        return NULL;
    }
    
    node->_value = calloc(VALUE_LENGTH, sizeof(char));
    
    if (!node->_value) {
        free(node->_name);
        free(node);
        return NULL;
    }
    
    return node;
}


void setName(TAttrNode *node, char *name)
{
    strcpy(node->_name, name);
}

void setValue(TAttrNode *node, char *value)
{
    strcpy(node->_value, value);
}

TAttrNode *createNode(char *name, char *value)
{
    TAttrNode *node = allocNode();
    
    if (!node)
        return NULL;
    
    setName(node, name);
    setValue(node, value);
    
    return node;
}

void append(TAttrList *list, TAttrNode *node)
{
    if ((*list) == NULL)
        *list = node;
    else {
        TAttrNode *aux_node = *list;
        
        while (aux_node->_next != NULL)
            aux_node = aux_node->_next;
        
        aux_node->_next = node;
    }
}

void deleteNode(TAttrNode *node)
{
    free(node->_name);
    free(node->_value);
    free(node);
}

void deleteList(TAttrList *list)
{
    if (!(*list))
        return;
    
    TAttrNode *aux_node = NULL;
    
    while (*list) {
        aux_node = *list;
        
        *list = (*list)->_next;
        
        deleteNode(aux_node);
    }

    free(*list);
    *list = NULL;
}

void printList(TAttrList list, FILE *output_file, int is_style)
{
    int is_first = 1;
    if (!is_style) {
        while (list) {
            fprintf(output_file, " %s=\"%s\"", list->_name, list->_value);
            list = list->_next;
        }
    } else {
        if (!list)
            return;
        
        fprintf(output_file, " style=\"");
        
        while (list) {
            if (is_first) {
                fprintf(output_file, "%s: %s;", list->_name, list->_value);
                is_first = 0;
            } else {
                fprintf(output_file, " %s: %s;", list->_name, list->_value);
            }
            list = list->_next;
        }
        
        fprintf(output_file, "\"");
    }
}

int searchAttribute(TAttrList list, char *attribute_name,
                    char *attribute_value)
{
    if (!list)
        return ATTRIBUTE_NOT_FOUND;
    else {
        while (list) {
            if (strcmp(list->_name, attribute_name) == 0) {
                if (attribute_value != NULL) {
                    if (strcmp(list->_value, attribute_value) == 0)
                        return ATTRIBUTE_FOUND;
                    else
                        return ATTRIBUTE_NOT_FOUND;
                } else {
                    return ATTRIBUTE_FOUND;
                }
            }
                
            list = list->_next;
        }
    }
    return ATTRIBUTE_NOT_FOUND;
}

TAttrList splitStyleString(char *style_string)
{
    TAttrList list = NULL;
    
    deleteWhitespaces(style_string);
    
    char *style_ptr = strtok(style_string, STYLE_SPLITTER);
    char name_buffer[1000];
    char value_buffer[1000];
    
    while (style_ptr) {
        
        sscanf(style_ptr, "%[^:] : %s", name_buffer, value_buffer);
        
        TAttrNode *node = createNode(name_buffer, value_buffer);
        if (!node) {
            deleteList(&list);
            return NULL;
        }
        append(&list, node);
        
        style_ptr = strtok(NULL, STYLE_SPLITTER);
    }
    
    return list;
}

void overrideStyle(TQueue *selector_result, char *style_string)
{
    char *style_string_copy = calloc(VALUE_LENGTH, sizeof(char));
    if (!style_string_copy)
        return;
    while (!QUEUE_EMPTY(selector_result)) {
        strcpy(style_string_copy, style_string);
        TAttrList style_list = splitStyleString(style_string_copy);
        TTree cur_node = dequeue(selector_result);
        
        deleteList(&cur_node->_info->_style);
        cur_node->_info->_style = style_list;
    }
    free(style_string_copy);
}

void update(TAttrList *list, TAttrNode *node)
{
    TAttrNode *copy_node = createNode(node->_name, node->_value);
    
    if (!copy_node)
        return;
    
    if (!*list)
        *list = copy_node;
    else {
        TAttrNode *iterator = *list;
        
        while (iterator->_next != NULL) {
            
            if (strcmp(iterator->_name, copy_node->_name) == 0) {
                setValue(iterator, copy_node->_value);
                deleteNode(copy_node);
                return;
            }
            iterator = iterator->_next;
        }
        if (strcmp(iterator->_name, copy_node->_name) == 0) {
            setValue(iterator, copy_node->_value);
            deleteNode(copy_node);
            return;
        } else
            iterator->_next = copy_node;
    }
}

void appendStyle(TQueue *selector_result, char *style_string)
{
    TAttrList style_list = splitStyleString(style_string);
   
    while (!QUEUE_EMPTY(selector_result)) {
        TTree cur_node = dequeue(selector_result);
        
        TAttrNode *style_list_iterator = style_list;
        
        while (style_list_iterator) {
            update(&cur_node->_info->_style, style_list_iterator);
            style_list_iterator = style_list_iterator->_next;
        }
    }
    deleteList(&style_list);
}
