#include "def.h"
#include "parser.h"
//parser

void epxand_if_necessary(){
    if(Comms_To_exec==Current_Comms_MAX){

            UserInput_Commands     = (char**)realloc(UserInput_Commands, sizeof(char*)*Current_Comms_MAX*2);
            UserInput_Args         = (char**)realloc(UserInput_Args,     sizeof(char*)*Current_Comms_MAX*2);
            File_Redirection_array = (char**)realloc(File_Redirection_array,     sizeof(char*)*Current_Comms_MAX*2);
            for(int k = Current_Comms_MAX; k<Current_Comms_MAX*2; k++){
                UserInput_Commands[k]     =       (char*)malloc(COMM_MAX_CHAR_INPUT);
                UserInput_Args[k]         =       (char*)malloc(Argument_Max_Char_Input);
                File_Redirection_array[k] =       (char*)malloc(255);
                File_Redirection_array[k][0] = '\0';
            }
            Current_Comms_MAX=Current_Comms_MAX*2;
    }
}


/*
*loop mexri na breis command, otan to vreis koita gia arguments, 3ana loop gia ta ypoloipa
*/
void parser(unsigned int nbytes){
    unsigned int i=0,comm_len=0,args_len=0;
    char parseInput[256];
    short has_arguments=0;
    pid_t pid;
    int red_counter;
    char flag;
    while(input_buffer[i]==' ')
        i++;    //ignore white spaces
    if(input_buffer[i]=='\n')return;   //no command


    while(i<nbytes){
        epxand_if_necessary();
        //command input
        while(input_buffer[i]==' ') 
            i++;

        while(  input_buffer[i]!=' ' && input_buffer[i]!='\n' && input_buffer[i]!=';' &&
                input_buffer[i]!='>' && input_buffer[i]!='<' && input_buffer[i]!= '|'){
            epxand_if_necessary();
            UserInput_Commands[Comms_To_exec][comm_len]=input_buffer[i];   //store command
            i++;
            comm_len++;
                if(comm_len>COMM_MAX_CHAR_INPUT-1){
                printf("Invalid command");
                exit(-1);
            }         
        }
        epxand_if_necessary();
        UserInput_Commands[Comms_To_exec][comm_len]='\0';
        comm_len=0;
        
        //ignore whitespaces
        while(input_buffer[i]==' ')
            i++;


        switch (input_buffer[i]){
        case ';':       //new command same line

            if(!has_arguments){
                UserInput_Args[Comms_To_exec][0]='\0';
                Comms_To_exec++;
            }
            epxand_if_necessary();
            has_arguments=0;
            i++;
            
            break;

        case '|':   //PIPELINE
            if(!has_arguments){
                UserInput_Args[Comms_To_exec][0]='\0';
                Comms_To_exec++;
            }
            UserInput_Commands[Comms_To_exec-1][COMM_MAX_CHAR_INPUT] = '|';
            epxand_if_necessary();
            has_arguments=0;
            i++;

            break;

        case '<': //REDIRECTION INPUT 0
                //ignore whitespaces
                if(!has_arguments){
                    UserInput_Args[Comms_To_exec][0]='\0';
                    Comms_To_exec++;
                    epxand_if_necessary();
                }
                has_arguments=0;
                i++;
                red_counter=0;
                while(input_buffer[i]==' ')
                    i++;
                while(input_buffer[i]!='\n' && input_buffer[i]!='|' &&
                input_buffer[i]!='>'&& input_buffer[i]!='<' && input_buffer[i]!=';'){
                    File_Redirection_array[Comms_To_exec-1][red_counter] = input_buffer[i];
                    i++;
                    red_counter++;
                }
                // -1 gt hdh exei au3ei8ei apo to default gia ta args
                File_Redirection_array[Comms_To_exec-1][red_counter]   = ' ' ;
                File_Redirection_array[Comms_To_exec-1][red_counter+1] = '0' ;
                File_Redirection_array[Comms_To_exec-1][red_counter+2] = '\0';


            break;

        case '>':   //REDIRECTION OUTPUT 1(>) or 11(>>)
            if(!has_arguments){
                UserInput_Args[Comms_To_exec][0]='\0';
                Comms_To_exec++;
                epxand_if_necessary();
            }
            has_arguments=0;

            flag = '1';
            i++;
            red_counter = 0;
            if (input_buffer[i]=='>'){
                flag = '2';
                i++;
            }
            while(input_buffer[i]==' ')
                i++;
            while(input_buffer[i]!='\n' && input_buffer[i]!='|' &&
                input_buffer[i]!='>'&& input_buffer[i]!='<' && input_buffer[i]!=';'){
                    File_Redirection_array[Comms_To_exec-1][red_counter] = input_buffer[i];
                    i++;
                    red_counter++;
                }
                File_Redirection_array[Comms_To_exec-1][red_counter]   = ' ' ;
                File_Redirection_array[Comms_To_exec-1][red_counter+1] = flag ;
                File_Redirection_array[Comms_To_exec-1][red_counter+2] = '\0';
            break;

        default:    //Arguments     [arg1 arg2 arg3\0]
            has_arguments=1;
            args_len=0; 
            while(input_buffer[i]!='\n' && input_buffer[i]!='|' &&
                input_buffer[i]!='>'&& input_buffer[i]!='<' && input_buffer[i]!=';'){
                
                if(input_buffer[i]==' '){
                    if(UserInput_Args[Comms_To_exec][args_len-1]!= ' '){
                        UserInput_Args[Comms_To_exec][args_len]=' ';
                        args_len++;

                    }
                    i++;

                    if(args_len>=Argument_Max_Char_Input-1){
                        for(int j=0; j<Current_Comms_MAX; j++){
                            UserInput_Args[j]=(char*)realloc(UserInput_Args[j],Argument_Max_Char_Input*2);
                        }
                    }
                    continue; //ignore whitespace
                }

                UserInput_Args[Comms_To_exec][args_len]=input_buffer[i];
                i++;
                args_len++;
                if(args_len>=Argument_Max_Char_Input-1){
                    for(int j=0; j<Current_Comms_MAX; j++){
                        UserInput_Args[j]=(char*)realloc(UserInput_Args[j],Argument_Max_Char_Input*2);
                    }
                    Argument_Max_Char_Input=Argument_Max_Char_Input*2;

                }
            }
            UserInput_Args[Comms_To_exec][args_len]='\0';
            Comms_To_exec++;
            epxand_if_necessary();
            break;
        }

    }

}