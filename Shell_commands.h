#ifndef SHELL_COMMANDS_H
#define SHELL_COMMANDS_H
#include "def.h"
#define SUPPORTED_COMMANDS 14
#define BUILD_INS 7
#define EXTERNAL 8
#define MIN_ARGUMENT_NUMBER_SYSTEM 5 

extern char** UserInput_Commands;  //array me commands
extern char** UserInput_Args;      //array me args


//returns -1 if the command wasnt build-in  
char** toSYSTEM_COMMAND();

int BuildIn_CommandHandler();

int External_CommandHandler();

int Execute_Command();

int Redirection_Handler(char* file_path);

int Redirection_Handler_BuildIns(char* filepath);  
#endif