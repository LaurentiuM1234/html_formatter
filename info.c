/*Mihalcea Laurentiu 313CB*/
#include "info.h"
#include "attribute.h"
#include "fop.h"


TInfo createInfo(void)
{
    TInfo info = calloc(1, sizeof(struct Info));
    
    if (!info)
        return NULL;
    
    info->_type = calloc(TYPE_SIZE, sizeof(char));
    
    if (!info->_type) {
        free(info);
        return NULL;
    }
    
    info->_id = calloc(ID_SIZE, sizeof(char));
    
    if (!info->_id) {
        free(info->_type);
        free(info);
        return NULL;
    }
    
    info->_contents = calloc(CONTENTS_SIZE, sizeof(char));
    
    if (!info->_contents) {
        free(info->_id);
        free(info->_type);
        free(info);
        return NULL;
    }
    
    return info;
}


void deleteInfo(TInfo *info)
{
    free((*info)->_contents);
    free((*info)->_id);
    free((*info)->_type);
    deleteList(&(*info)->_other_attributes);
    deleteList(&(*info)->_style);
    free(*info);
    
    *info = NULL;
}


void catContent(TInfo info, char *character)
{
    strcat(info->_contents, character);
}

void catType(TInfo info, char *character)
{
    strcat(info->_type, character);
}

void setID(TInfo info, char *id)
{
    strcpy(info->_id, id);
}


void formatContents(TInfo info)
{
    char *aux_string = calloc(CONTENTS_SIZE, sizeof(char));
    
    if (!aux_string)
        return;
    
    char *c_ptr = strtok(info->_contents, WHITESPACE);
    
    while (c_ptr) {
        strcat(aux_string, c_ptr);
        strcat(aux_string, " ");
        c_ptr = strtok(NULL, WHITESPACE);
    }
    aux_string[strlen(aux_string) - 1] = '\0';
    strcpy(info->_contents, aux_string);
    free(aux_string);
}
