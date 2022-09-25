#include "def.h"
const char* USER;
char CWD[PATH_MAX];


unsigned int Current_Comm_inline;

char *input_buffer;
unsigned int buffer_len = 0; //Dont touch
unsigned int Comms_To_exec=0;

char** File_Redirection_array;
char** UserInput_Commands;  //array me commands
char** UserInput_Args;      //array me args
unsigned int Argument_Max_Char_Input=COMM_MAX_CHAR_INPUT;  //init args 
unsigned int Current_Comms_MAX=INIT_NUM_COMM;   //max comands sto array commands
