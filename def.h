//csd4193 Giorgos Dovas
//Globals all capital
#ifndef DEF_H
#define DEF_H
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define INPUT_SIZE_BUFFER
#define PATH_MAX 2048
#define clear() printf("\033[H\033[J") //To phra etoimo e4axna enallaktikh gia na katharizei ta prohgoymena
#define INIT_NUM_COMM 5                //init number of commands, it can change
#define TRUE 1
#define FALSE 0
#define COMM_MAX_CHAR_INPUT 42 + 1


extern const char* USER;
extern char CWD[PATH_MAX];

extern char *input_buffer;
extern unsigned int buffer_len;  //Dont touch
extern unsigned int Comms_To_exec;
extern unsigned int Current_Comm_inline;
extern char**        File_Redirection_array;
extern char**        UserInput_Commands;  //array me commands
extern char**        UserInput_Args;      //array me args
extern unsigned int Argument_Max_Char_Input;    //init args 
extern unsigned int Current_Comms_MAX;          //max comands sto array commands


void epxand_if_necessary();


#endif