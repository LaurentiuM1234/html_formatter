#ifndef __FILE_OPERATIONS__
#define __FILE_OPERATIONS__

#include "utils.h"

#define PARSER_SIZE 2
#define WHITESPACE " "
#define TMP_NAME_SIZE 1000
#define TMP_VALUE_SIZE 1000
#define INPUT_STRING_SIZE 100 * 1024
#define CMD_SIZE 20
#define FIRST_ARG_SIZE 51190
#define SECOND_ARG_SIZE 51190
#define OUT_BUFFER_SIZE 1000

#define FORMAT 0
#define ADD 1
#define REC_DEL 2
#define O_STYLE 3
#define A_STYLE 4

#define CLASS_SELECTOR 0
#define ELEMENT_SELECTOR 1
#define EC_SELECTOR 2
#define EE_SELECTOR 3
#define EPE_SELECTOR 4
#define ID_SELECTOR 5

/*Function used to construct a tree strcture from
 specified file's contents(allocation is implicit)
 @params: file_path:->string containing the file's path
 @returns: pointer to newly created tree structure*/
TTree readTree(const char *file_path);

/*Function used to output a tree to a specified file
 @params: root:->root of the tree
          output_file:->pointer to already opened out
 file
          tab_num:->number of tabs to output before each
 tag*/
void saveTree(TTree root, FILE *output_file, int tab_num);

/*Function used to output a specified number of tabs
 @params: output_file:->pointer to already opened out
 file
          tab_num:->number of tabs to output*/
void printTabs(FILE *output_file, int tab_num);

/*Function used to create a node from a tag
 @params: tag_string:->string containing the tag from
 which the node is created
 @returns: pointer to the newly created tag node or NULL
 if any of the allocations failed*/
TTreeNode *readTag(const char *tag_string);

/*Function containing the main loop of the program
 @params: html_file:->string containing the path to the
 html file
          command_file:->string containing the path
 to the command file
          output_file:->string containing the path
 to the output file*/
void startProgram(const char *html_file,
                  const char *command_file, const char *output_file);

/*Function used to process the input from the command file
 @params: input_string:->string containing one line of
 input from the command file
           output:->pointer to already opened out file
           root:->root of the tree on which the operations
 are done*/
void processInput(char *input_string, FILE *output, TTree root);

/*Function used to turn a command string into an integer
 code
 @params: command:->the command string
 @returns: one of the 5 command defines*/
int encodeCommand(char *command);

/*Function used to implement the add command specified in
 task
 @params: root:->root of the tree
          first_argument:->string containing the first
 argument of the input string
          second_argument:->string containing the second
 argument of the input string
          output:->pointer to already opened out file
 !!NOTE:->each input string is made of a command, a first
 argument and a second argument(both optional)!!*/
void addCommand(TTree root, char *first_argument,
                char *second_argument, FILE *output);

/*Function used to implement the delete command specified
 in task
 @params: root:->root of the tree
          first_argument:->string containing the first
 argument of the input string
          output:->pointer to already opened out file*/
void deleteCommand(TTree root, char *first_argument, FILE *output);

/*Function used to implement the override command
specified in task
@params: root:->root of the tree
         first_argument:->string containing the first
argument of the input string
         second_argument:->string containing the second
argument of the input string
         output:->pointer to already opened out file
!!NOTE:->each input string is made of a command, a first
argument and a second argument(both optional)!!*/
void overrideCommand(TTree root, char *first_argument,
                     char *second_argument, FILE *output);

/*Function used to implement the append command
specified in task
@params: root:->root of the tree
         first_argument:->string containing the first
argument of the input string
         second_argument:->string containing the second
argument of the input string
         output:->pointer to already opened out file
!!NOTE:->each input string is made of a command, a first
argument and a second argument(both optional)!!*/
void appendCommand(TTree root, char *first_argument,
                   char *second_argument, FILE *output);

/*Function used to delete all whitespaces in a string
 @params: input_string:->the string which will have
 all of its whitespaces deleted*/
void deleteWhitespaces(char *input_string);

/*Function used to split an argument string into 2
 separate ones
 @params: first_argument:->string which will contain
 the first part of the argument
          second_argument:->string which will contain
 the second part of the argument
          argument:->string containing the argument*/
void splitArgument(char *first_argument,
                   char *second_argument, char *argument);

/*Function used to turn a selector string into an
 integer code
 @params: selector:->string containing the selector
 @returns: one of the 6 selector defines*/
int encodeSelector(const char *selector);

/*Function used to apply a selector on a specified tree
 @params: selector:->string containing the selector
 which will be applied
          root:->root of the tree
 @returns: queue containing the selector result or NULL
 if any of the allocations failed*/
TQueue *applySelector(char *selector, TTree root);




#endif
