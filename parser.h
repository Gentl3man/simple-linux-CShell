#ifndef PARSER_H
#define PARSER_H

//declare
extern char *input_buffer;
extern unsigned int buffer_len ; //Dont touch
extern unsigned int Comms_To_exec;
extern char** UserInput_Commands;  //array me commands
extern char** UserInput_Args;      //array me args
extern unsigned int Argument_Max_Char_Input;    //init args 
extern unsigned int Current_Comms_MAX;          //max comands sto array commands

void parser(unsigned int nbytes);

#endif