//Author: Giorgos Dovas
//Globals all capital
#include "parser.h"
#include "def.h"
#include "Shell_commands.h"
void promtp(){
    printf("%s@cs345sh%s$ ",USER,CWD); //TODO des gia th 8esh dir
}

void init_shell(){
    clear();
    printf("\n\n\n********************************************\n");
    printf("\n\n***************\t\tShell made by Giorgos Dovas\t\t***************\n\n");
    printf("\n\n\n***************\tSafe to use? probably not. Use it with a vm ***************\n\n\n");
    USER = getlogin();
    getcwd(CWD,PATH_MAX);
    UserInput_Commands      = (char**)malloc(sizeof(char*)*INIT_NUM_COMM  );
    UserInput_Args          = (char**)malloc(sizeof(char*)*INIT_NUM_COMM  );
    File_Redirection_array  = (char**)malloc(sizeof(char*)*INIT_NUM_COMM  );
    

    for(int i=0; i<INIT_NUM_COMM; i++){
        UserInput_Commands[i]   = (char*)malloc(COMM_MAX_CHAR_INPUT);
        UserInput_Args[i]       = (char*)malloc(COMM_MAX_CHAR_INPUT);
        File_Redirection_array[i]  = (char*)malloc(255);

        File_Redirection_array[i][0]='\0';
        UserInput_Args[i][0]='\0';
    }
    
    
}


void wait_for_input(){
    ssize_t nread;
    Comms_To_exec=0;
    Current_Comm_inline=0;
    for(int i=0; i<Current_Comms_MAX; i++){
        UserInput_Commands[i][0]=   '\0';
        UserInput_Args[i][0]    =   '\0';
    }
    nread = getline(&input_buffer, &buffer_len, stdin)-1; //-1 cause getline stores \n aswell 
    if(nread<0){
        perror("Need to restart shell");
        exit(0);
    }
    parser(nread);

    

}

void clean_pipes(){
    for(int i =0; i< Current_Comms_MAX; i++){
        UserInput_Commands[i][COMM_MAX_CHAR_INPUT]='\0';
    }
}


void main(){
    
    init_shell();
    while(TRUE){
        promtp();
        wait_for_input();
        while(Current_Comm_inline<Comms_To_exec){
            Execute_Command();
            Current_Comm_inline++;
        }
        clean_pipes();
    }
}