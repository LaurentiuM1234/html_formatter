/*Mihalcea Laurentiu 313CB*/
#include "fop.h"
#include "stack.h"
#include "tree.h"
#include "info.h"
#include "attribute.h"
#include "selector.h"
#include "queue.h"


TTree readTree(const char *file_path)
{
    FILE *input_file = fopen(file_path, "r");
    
    if (!input_file)
        return NULL;
    
    char *parser = calloc(PARSER_SIZE, sizeof(char));
    
    if (!parser)
        return NULL;
    
    char *tmp_name = calloc(TMP_NAME_SIZE, sizeof(char));
    
    if (!tmp_name)
        return NULL;
    
    char *tmp_value = calloc(TMP_VALUE_SIZE, sizeof(char));
    
    if (!tmp_value) {
        free(tmp_name);
        return NULL;
    }
    
    TTree root = NULL;
    TTree node = NULL;
    TParseState current_state = PARSE_CONTENTS;
    TParseState next_state;
    TStack *stack = createStack(sizeof(TTree));
    
    if (!stack)
        return NULL;
    
    while (fgets(parser, PARSER_SIZE, input_file) != NULL) {
        
        next_state = Interpret(current_state, *parser);
        
        if (current_state == PARSE_CONTENTS &&
            next_state == PARSE_CONTENTS) {
            if (*parser != '\n' && *parser != '\r')
                addContent(getTop(stack), parser);
        }
        
        if (current_state == PARSE_OPENING_BRACKET &&
            next_state == PARSE_TAG_TYPE) {
            node = createEmptyNode();
            addType(node, parser);
            push(stack, node);
        }
        
        if ((current_state == PARSE_CLOSING_TAG &&
             next_state == PARSE_CONTENTS) ||
            (current_state == PARSE_SELF_CLOSING &&
             next_state == PARSE_CONTENTS)) {
            TTree top_node = pop(stack);
            
            if (current_state == PARSE_SELF_CLOSING &&
                next_state == PARSE_CONTENTS)
                top_node->_info->_is_self_closing = 1;
            
            if (STACK_EMPTY(stack)) {
                root = top_node;
                
                deleteStack(&stack, NULL);
                free(tmp_name);
                free(tmp_value);
                free(parser);
                fclose(input_file);
                return root;
            }else {
                addChild(getTop(stack), top_node, 0);
            }
        }
        if (current_state == PARSE_TAG_TYPE &&
            next_state == PARSE_TAG_TYPE)
            addType(getTop(stack), parser);
        
        if ((current_state == PARSE_ATTRIBUTE_NAME &&
             next_state == PARSE_ATTRIBUTE_NAME) ||
            (current_state == PARSE_REST_OF_TAG &&
             next_state == PARSE_ATTRIBUTE_NAME))
            strcat(tmp_name, parser);
        
        if (current_state == PARSE_ATTRIBUTE_VALUE &&
            next_state == PARSE_ATTRIBUTE_VALUE)
            strcat(tmp_value, parser);
        
        if (current_state == PARSE_ATTRIBUTE_VALUE &&
            next_state == PARSE_REST_OF_TAG) {
            TAttrNode *attribute = createNode(tmp_name, tmp_value);
            memset(tmp_name, '\0', TMP_NAME_SIZE);
            memset(tmp_value, '\0', TMP_VALUE_SIZE);
            TTree top_node = getTop(stack);
            
            if (!attribute) {
                deleteStack(&stack, purgeTreeHandler);
                free(tmp_name);
                free(tmp_value);
                free(parser);
                fclose(input_file);
                return NULL;
            }
            
            if (strcmp(attribute->_name, "style") == 0) {
                top_node->_info->_style = splitStyleString(attribute->_value);
                deleteNode(attribute);
            }
            else
                append(&top_node->_info->_other_attributes, attribute);
            
        }
        current_state = next_state;
    }
    
    deleteStack(&stack, NULL);
    free(tmp_name);
    free(tmp_value);
    free(parser);
    fclose(input_file);
    
    return root;
}


void saveTree(TTree root, FILE *output_file, int tab_num)
{
    if (!root)
        return;
    
    printTabs(output_file, tab_num);
    fprintf(output_file, "<%s", root->_info->_type);
    printList(root->_info->_style, output_file, 1);
    printList(root->_info->_other_attributes, output_file, 0);
    
    if (root->_info->_is_self_closing)
        fprintf(output_file, "/>\n");
    else
        fprintf(output_file, ">\n");
    
    if (*root->_info->_contents != '\0') {
        printTabs(output_file, tab_num + 1);
        formatContents(root->_info);
        fprintf(output_file, "%s\n", root->_info->_contents);
    }
    
    saveTree(root->_first_child, output_file, tab_num + 1);
    
    if (!root->_info->_is_self_closing) {
        printTabs(output_file, tab_num);
        fprintf(output_file, "</%s>\n", root->_info->_type);
    }
    
    saveTree(root->_next_sibling, output_file, tab_num);
    
}

void printTabs(FILE *output_file, int tab_num)
{
    for (int i = 0; i < tab_num; i++)
        fprintf(output_file, "\t");
}

TTreeNode *readTag(const char *tag_string)
{
    char *tmp_name = calloc(TMP_NAME_SIZE, sizeof(char));
    
    if (!tmp_name)
        return NULL;
    
    char *tmp_value = calloc(TMP_VALUE_SIZE, sizeof(char));
    
    if (!tmp_value) {
        free(tmp_name);
        return NULL;
    }
    
    char *parser = calloc(PARSER_SIZE, sizeof(char));
    
    if (!parser) {
        free(tmp_name);
        free(tmp_value);
        return NULL;
    }
    
    TTreeNode *node = NULL;
    TParseState current_state = PARSE_CONTENTS;
    TParseState next_state;
    
    for (int i = 0; *(tag_string + i) != '\0'; i++) {
        sprintf(parser, "%c", tag_string[i]);
        
        next_state = Interpret(current_state, *parser);
        
        if (current_state == PARSE_CONTENTS &&
            next_state == PARSE_CONTENTS) {
            if (*parser != '\n' && *parser != '\r')
                addContent(node, parser);
        }
        if (current_state == PARSE_OPENING_BRACKET &&
            next_state == PARSE_TAG_TYPE) {
            node = createEmptyNode();
            addType(node, parser);
        }
        if (current_state == PARSE_SELF_CLOSING &&
            next_state == PARSE_CONTENTS)
            node->_info->_is_self_closing = 1;
        
        if (current_state == PARSE_TAG_TYPE &&
            next_state == PARSE_TAG_TYPE)
            addType(node, parser);
        
        if ((current_state == PARSE_ATTRIBUTE_NAME &&
             next_state == PARSE_ATTRIBUTE_NAME) ||
            (current_state == PARSE_REST_OF_TAG &&
             next_state == PARSE_ATTRIBUTE_NAME))
            strcat(tmp_name, parser);
        
        if (current_state == PARSE_ATTRIBUTE_VALUE &&
            next_state == PARSE_ATTRIBUTE_VALUE)
            strcat(tmp_value, parser);
        
        if (current_state == PARSE_ATTRIBUTE_VALUE &&
            next_state == PARSE_REST_OF_TAG) {
            TAttrNode *attribute = createNode(tmp_name, tmp_value);
            
            if (!attribute) {
                free(tmp_name);
                free(tmp_value);
                free(parser);
                return NULL;
            }
            
            if (strcmp(attribute->_name, "style") == 0) {
                node->_info->_style = splitStyleString(attribute->_value);
                deleteNode(attribute);
            }
            else
                append(&node->_info->_other_attributes, attribute);
            
        }
        
        current_state = next_state;
    }
    free(tmp_name);
    free(tmp_value);
    free(parser);
    return node;
}


void startProgram(const char *html_file,
                  const char *command_file, const char *output_file)
{
    FILE *input = fopen(command_file, "r");
    FILE *output = fopen(output_file, "w");
    
    if (!input)
        return;
    if (!output) {
        fclose(input);
        return;
    }
    
    TTree root = readTree(html_file);
    
    if (!root) {
        fclose(input);
        fclose(output);
        return;
    }
    setTreeID(root);
    
    char *input_string = calloc(INPUT_STRING_SIZE, sizeof(char));
    
    if (!input_string) {
        purgeTree(&root);
        fclose(input);
        fclose(output);
        return;
    }
    
    int line_number = 0;
    
    fscanf(input, "%d\n", &line_number);
    
    for (int i = 0; i < line_number; i++) {
        fgets(input_string, INPUT_STRING_SIZE, input);
        input_string[strlen(input_string) - 1] = '\0';
        processInput(input_string, output, root);
    }
    
    free(input_string);
    purgeTree(&root);
    fclose(input);
    fclose(output);
}

void processInput(char *input_string, FILE *output, TTree root)
{
    char *argument = calloc(FIRST_ARG_SIZE +
                            SECOND_ARG_SIZE, sizeof(char));
    
    if (!argument)
        return;
    
    char *command = calloc(CMD_SIZE, sizeof(char));
    
    if (!command) {
        fclose(output);
        return;
    }
    
    char *first_argument = calloc(FIRST_ARG_SIZE, sizeof(char));
    
    if (!first_argument) {
        fclose(output);
        free(command);
        free(argument);
        return;
    }
    
    char *second_argument = calloc(SECOND_ARG_SIZE, sizeof(char));
    
    if (!second_argument) {
        fclose(output);
        free(first_argument);
        free(argument);
        free(command);
        return;
    }
    
    sscanf(input_string, "%[^ ] %[^\n]", command, argument);
    
    if (*argument != '\0')
        splitArgument(first_argument, second_argument, argument);
    
    switch(encodeCommand(command)) {
    case FORMAT: {
        saveTree(root, output, 0);
        
    }break;
    case ADD: {
        addCommand(root, first_argument, second_argument, output);
            
    }break;
    case REC_DEL: {
        deleteCommand(root, first_argument, output);
    }break;
    case O_STYLE: {
        overrideCommand(root, first_argument, second_argument, output);
    }break;
    case A_STYLE: {
        appendCommand(root, first_argument, second_argument, output);
    }break;
    }
    free(first_argument);
    free(second_argument);
    free(command);
    free(argument);
}

int encodeCommand(char *command)
{
    if (strcmp(command, "format") == 0)
        return FORMAT;
    else if (strcmp(command, "add") == 0)
        return ADD;
    else if (strcmp(command, "deleteRecursively") == 0)
        return REC_DEL;
    else if (strcmp(command, "overrideStyle") == 0)
        return O_STYLE;
    else
        return A_STYLE;
}

void addCommand(TTree root, char *first_argument,
                char *second_argument, FILE *output)
{
    char *subcommand_name = calloc(ID_SIZE, sizeof(char));
    char *subcommand_value = calloc(ID_SIZE, sizeof(char));
    
    if (!subcommand_name) {
        return;
    }
    
    if (!subcommand_value) {
        free(subcommand_name);
        return;
    }
    sscanf(first_argument, "%[^=] = %s", subcommand_name, subcommand_value);
    
    TQueue *queue = createQueue(sizeof(TTree));
    
    if (!queue) {
        free(subcommand_name);
        free(subcommand_value);
        return;
    }
    
    IDSelect(root->_first_child, subcommand_value, queue, 0);
    
    if (QUEUE_EMPTY(queue)) {
        fprintf(output, "Add tag failed: node with id %s not found!\n",
                subcommand_value);
        free(subcommand_name);
        free(subcommand_value);
        deleteQueue(&queue, NULL);
        return;
    }
    
    sscanf(second_argument, "%[^=] =\"%[^\"]", subcommand_name,
           subcommand_value);
    
    TTree node = readTag(subcommand_value);
    
    if (!node) {
        free(subcommand_name);
        free(subcommand_value);
        dequeue(queue);
        deleteQueue(&queue, NULL);
        return;
    }
    
    addChild(dequeue(queue), node, 1);
    
    free(subcommand_name);
    free(subcommand_value);
    deleteQueue(&queue, NULL);
}

void deleteCommand(TTree root, char *first_argument, FILE *output)
{
    char *subcommand_name = calloc(ID_SIZE, sizeof(char));
    char *subcommand_value = calloc(ID_SIZE, sizeof(char));
    
    if (!subcommand_name) {
        return;
    }
    
    if (!subcommand_value) {
        free(subcommand_name);
        return;
    }
    
    sscanf(first_argument, "%[^=] = %[^\n]", subcommand_name, subcommand_value);
    
    TQueue *selector_queue = applySelector(subcommand_value, root);
    
     if (QUEUE_EMPTY(selector_queue)) {
        fprintf(output, "Delete recursively failed: no node found for selector %s!\n",
                subcommand_value);
        free(subcommand_name);
        free(subcommand_value);
        deleteQueue(&selector_queue, NULL);
        return;
    }
    
    recursiveDelete(root, selector_queue);
    setTreeID(root);
    
    deleteQueue(&selector_queue, NULL);
    free(subcommand_name);
    free(subcommand_value);
}


int encodeSelector(const char *selector)
{
    if (selector[0] == '.')
        return CLASS_SELECTOR;
    else if(selector[0] == '#')
        return ID_SELECTOR;
    else if (strchr(selector, '>') != NULL)
        return EPE_SELECTOR;
    else if (strchr(selector, ' ') != NULL)
        return EE_SELECTOR;
    else if (strchr(selector, '.') != NULL)
        return EC_SELECTOR;
    else
        return ELEMENT_SELECTOR;
}

TQueue *applySelector(char *selector, TTree root)
{
    TQueue *selector_queue = NULL;
    
    switch (encodeSelector(selector)) {
    case CLASS_SELECTOR: {
        selector_queue = classSelect(root, selector + 1);
    }break;
    case EC_SELECTOR: {
        char *element = calloc(TYPE_SIZE, sizeof(char));
        
        if (!element)
            return NULL;
        
        char *class = calloc(CONTENTS_SIZE, sizeof(char));
        
        if (!class) {
            free(element);
            return NULL;
        }
        
        sscanf(selector, "%[^.] . %s", element, class);
        
        selector_queue = ecSelector(root, element, class);
        free(element);
        free(class);
    }break;
    case EPE_SELECTOR: {
        char *element1 = calloc(TYPE_SIZE, sizeof(char));
        
        if (!element1)
            return NULL;
        
        char *element2 = calloc(CONTENTS_SIZE, sizeof(char));
        
        if (!element2) {
            free(element1);
            return NULL;
        }
        
        sscanf(selector, "%[^>] > %s", element1, element2);
        
        selector_queue = epeSelector(root, element1, element2);
        free(element1);
        free(element2);
            
    }break;
    case EE_SELECTOR: {
        char *element1 = calloc(TYPE_SIZE, sizeof(char));
        
        if (!element1)
            return NULL;
        
        char *element2 = calloc(CONTENTS_SIZE, sizeof(char));
        
        if (!element2) {
            free(element1);
            return NULL;
        }
        
        sscanf(selector, "%[^ ] %s", element1, element2);
        
        selector_queue = eeSelector(root, element1, element2);
        free(element1);
        free(element2);
    }break;
    case ELEMENT_SELECTOR: {
        selector_queue = elementSelector(root, selector);
    }break;
    case ID_SELECTOR: {
        selector_queue = createQueue(sizeof(TTree));
        
        if (!selector_queue)
            return NULL;
        
        IDSelect(root->_first_child, selector + 1, selector_queue, 0);
        
    }
    }
    return selector_queue;
}

void overrideCommand(TTree root, char *first_argument,
                     char *second_argument, FILE *output)
{
    char *subcommand_name = calloc(ID_SIZE, sizeof(char));
    char *subcommand_value = calloc(ID_SIZE, sizeof(char));
    
    if (!subcommand_name) {
        return;
    }
    
    if (!subcommand_value) {
        free(subcommand_name);
        return;
    }
    
    sscanf(first_argument, "%[^=] = %[^\n]", subcommand_name,
           subcommand_value);
    
    TQueue *selector_queue = applySelector(subcommand_value, root);
    
    if (QUEUE_EMPTY(selector_queue)) {
        fprintf(output, "Override style failed: no node found for selector %s!\n",
                subcommand_value);
        free(subcommand_name);
        free(subcommand_value);
        deleteQueue(&selector_queue, NULL);
        return;
    }
    
    sscanf(second_argument, "%[^=] =\"%[^\"]", subcommand_name,
           subcommand_value);
    
    overrideStyle(selector_queue, subcommand_value);
    
    deleteQueue(&selector_queue, NULL);
    free(subcommand_name);
    free(subcommand_value);
}

void appendCommand(TTree root, char *first_argument,
                   char *second_argument, FILE *output)
{
    char *subcommand_name = calloc(ID_SIZE, sizeof(char));
    char *subcommand_value = calloc(ID_SIZE, sizeof(char));
    
    if (!subcommand_name) {
        return;
    }
    
    if (!subcommand_value) {
        free(subcommand_name);
        return;
    }
    
    sscanf(first_argument, "%[^=] = %[^\n]", subcommand_name,
           subcommand_value);
    
    TQueue *selector_queue = applySelector(subcommand_value, root);
    
    if (QUEUE_EMPTY(selector_queue)) {
        fprintf(output, "Append to style failed: no node found for selector %s!\n",
                subcommand_value);
        free(subcommand_name);
        free(subcommand_value);
        deleteQueue(&selector_queue, NULL);
        return;
    }
    
    sscanf(second_argument, "%[^=] =\"%[^\"]", subcommand_name,
           subcommand_value);
    
    appendStyle(selector_queue, subcommand_value);
    
    deleteQueue(&selector_queue, NULL);
    free(subcommand_name);
    free(subcommand_value);
}

void deleteWhitespaces(char *input_string)
{
    char *aux_string = calloc(VALUE_LENGTH, sizeof(char));
    
    if (!aux_string)
        return;
    
    char *c_ptr = strtok(input_string, WHITESPACE);
    
    while (c_ptr) {
        strcat(aux_string, c_ptr);
        c_ptr = strtok(NULL, WHITESPACE);
    }
    strcpy(input_string, aux_string);
    free(aux_string);
}

void splitArgument(char *first_argument, char *second_argument,
                   char *argument)
{
    int i, j;
    int stop_flag = 1;
    int quote_flag = 0;
    
    for (i = 0, j = 0; stop_flag; i++) {
        if (argument[i] == '"')
            quote_flag += 1;
        else if ((argument[i] == ' ' && quote_flag == 0) || quote_flag == 2)
            stop_flag = 0;
        else {
            first_argument[j] = argument[i];
            j++;
        }
    }
    
    sscanf(argument + i, "%[^\n]", second_argument);
}

