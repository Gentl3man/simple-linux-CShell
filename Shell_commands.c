#include "Shell_commands.h"

unsigned int Current_Arg_Num_System = MIN_ARGUMENT_NUMBER_SYSTEM;

char Supported_Commands_External[EXTERNAL][COMM_MAX_CHAR_INPUT] = {
                        "ls"        ,
                        "ls -l"     ,   
                        "mkdir"     ,   //external
                        "sort"      ,   //external
                        "head"      ,   //external
                        "End it"    ,   //
                        "cat"           //external
};


//built-ins baby, gotta execute them all
//TODO RETURN VALUE
char BuildIns[BUILD_INS][COMM_MAX_CHAR_INPUT] = {
                    "cd"            ,
                    "help"          ,
                    "clear"         ,
                    "exit"          ,
                    "echo"          ,
                    "whoami"        ,
                    "pwd"
};



void Execute_pipeline(int pipes_num, int isParent){
    char** system_command;
    int pipefd[2];
    pid_t p1, p2;
    if(pipe(pipefd)<0){
        printf("pipe failed");
        exit(EXIT_FAILURE);
    }
    p1 = fork();
    if(p1 < 0 ){
        perror ("Frok failed\n");
        exit(EXIT_FAILURE);
    }
    if(p1==0){  //child 1  WRITES
        close(pipefd[0]);
        dup2 (pipefd[1],STDOUT_FILENO);
        close(pipefd[1]);
        Current_Comm_inline--;
        pipes_num--;

        if(pipes_num>0){
            Execute_pipeline(pipes_num,0);
        }else{
            if(BuildIn_CommandHandler()!=-1){
                exit(EXIT_SUCCESS);
            }
            system_command = toSYSTEM_COMMAND();
            if (execvp(system_command[0], system_command) < 0) {
                perror("Command not found\n");
                exit(-1);
		    }
            exit(EXIT_FAILURE);
        }
    
    }else{
        p2 = fork();
        if(p2<0){
            perror("Fork failed\n");
            exit(EXIT_FAILURE);
        }
        if(p2==0){  //child2 READS
             close(pipefd[1]);
             dup2 (pipefd[0],STDIN_FILENO);
             close(pipefd[0]);
            //Can execute now
            if(BuildIn_CommandHandler()!=-1){
                exit(EXIT_SUCCESS);
            }
            system_command = toSYSTEM_COMMAND();
            if (execvp(system_command[0], system_command) < 0) {
                exit(-1);
		    }
		    exit(-1);
        }else{//parent me 2 paidia ara 2 wait
            wait(NULL);
            if(isParent){
                wait(NULL);
            }else{
                exit(EXIT_SUCCESS);
            }

        }
    }
}


//It works DONT TOUCH
int BuildIn_CommandHandler(){
    
    int com = -1;
    pid_t pid = -2;
    int fd;
    int redir_flag=-1;
    char filepath[256];//must

    for(int i=0; i< BUILD_INS; i++){
        if(strcmp(UserInput_Commands[Current_Comm_inline],BuildIns[i])==0){
            com = i;
            break;
        }
    }
    if(com == -1)
        return com;

    redir_flag = Redirection_Handler_BuildIns(filepath);
    //Could be much better but i had no time... welp... it works though :)
    switch(com){
        case 0: //cd
            //ignore redirection
            chdir(UserInput_Args[Current_Comm_inline]);
            getcwd(CWD,PATH_MAX);

            break;

        case 1: //help
            if(redir_flag!=-1){
                pid = fork();

                if(pid==-1){
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    printf("Fork failed\n");
                    return -2;
                }else if(pid == 0){ //child
                //==================***Child code BEGIN***======================
                    if(redir_flag==1){
                        fd = open(filepath,O_WRONLY | O_APPEND | O_TRUNC);
                    }else{
                        fd = open(filepath,O_WRONLY | O_APPEND);
                    }
                    if(fd==-1){
                        //file doesnt exits create a new file
                        fd = open(filepath,O_CREAT | O_WRONLY , S_IRWXU);
                    }
                    dup2(fd,1); //Reminder: 1 to stdout


                //==================***Child code END***======================

                }else{//parent
                    wait(NULL);
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    return com;
                }
            }

            printf("\nShell available commands:\n");
            printf("***\t\t\tBuild-ins\t\t\t***\n");
            for(int i=0; i<BUILD_INS; i++){
                printf("> %s\n",BuildIns[i]);
            }
            printf("***\t\t\tExternal\t\t\t***\n");
            for(int i=0; i<EXTERNAL; i++){
                printf("> %s\n",Supported_Commands_External[i]);
            }
            break;

        case 2: //clear
            //ignore redirection
            clear();
            break;

        case 3: //exit
            //ignore redirection
            printf("\n\n***\t\t\tSHELL END\t\t\t***\n\n");
            exit(1);
        
        case 4: //echo

            if(redir_flag!=-1){
                pid = fork();
                if(pid==-1){
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    printf("Fork failed\n");
                    return -2;
                }else if(pid == 0){ //child
                //==================***Child code BEGIN***======================
                    if(redir_flag==1){
                        fd = open(filepath,O_WRONLY | O_APPEND | O_TRUNC);
                    }else{
                        fd = open(filepath,O_WRONLY | O_APPEND);
                    }
                    if(fd==-1){
                        //file doesnt exits create a new file
                        fd = open(filepath,O_CREAT | O_WRONLY , S_IRWXU);
                        
                    }
                    dup2(fd,1); //Reminder: 1 to stdout


                //==================***Child code END***======================

                }else{//parent
                    wait(NULL);
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    return com;
                }
            }


            printf("%s\n", UserInput_Args[Current_Comm_inline]);    //easy money
            break;
        
        case 5: //whoami? Who are u? why am i?

        if(redir_flag!=-1){
                pid = fork();
                
                if(pid==-1){
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    printf("Fork failed\n");
                    return -2;
                }else if(pid == 0){ //child
                //==================***Child code BEGIN***======================
                    if(redir_flag==1){
                        fd = open(filepath,O_WRONLY | O_APPEND | O_TRUNC);
                    }else{
                        fd = open(filepath,O_WRONLY | O_APPEND);
                    }
                    if(fd==-1){
                        //file doesnt exits create a new file
                        fd = open(filepath,O_CREAT | O_WRONLY , S_IRWXU);
                    }
                    dup2(fd,1); //Reminder: 1 to stdout


                //==================***Child code END***======================

                }else{//parent
                    wait(NULL);
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    return com;
                }
            }


            printf("%s\n",USER);
            break;
        
        case 6: //pwd
            if(redir_flag!=-1){
                pid = fork();
            
                if(pid==-1){
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    printf("Fork failed\n");
                    return -2;
                }else if(pid == 0){ //child
                //==================***Child code BEGIN***======================
                    if(redir_flag==1){
                        fd = open(filepath,O_WRONLY | O_APPEND | O_TRUNC);
                    }else{
                        fd = open(filepath,O_WRONLY | O_APPEND);
                    }
                    if(fd==-1){
                        //file doesnt exits create a new file
                        fd = open(filepath,O_CREAT | O_WRONLY , S_IRWXU);
                    }
                    dup2(fd,1); //Reminder: 1 to stdout


                //==================***Child code END***======================

                }else{//parent
                    wait(NULL);
                    File_Redirection_array[Current_Comm_inline][0]='\0';
                    return com;
                }
            }


            printf("dir: %s\n",CWD);
            break;

        default:
            //ignore redirection
            printf("Not a build-in command\n");
            break;
    }
    if(pid==0){
        exit(1);
    }
    return com;
}




int External_CommandHandler(){
    int com = -1;
    for (int i=0; i<EXTERNAL; i++){
        if(strcmp(UserInput_Commands[0],Supported_Commands_External[i])==0){
            com = i;
            break;
        }
    }

    switch (com){
        case 0:
        
            break;
    
        default:
            printf("Unknown command '%s'\n",UserInput_Commands[0]);
            break;
    }
}

char** toSYSTEM_COMMAND(){
    char** System_command=(char**)malloc(sizeof(char*)*Current_Arg_Num_System);
    System_command[0]=(char*)malloc(COMM_MAX_CHAR_INPUT);
    char* temp;
    int k=0;//Dont touch
    int i=0;//Dont touch
    int j=0;//Dont touch

    while(UserInput_Commands[Current_Comm_inline][i]!='\0'){
        System_command[0][i]=UserInput_Commands[Current_Comm_inline][i];
        i++;
    }
    System_command[0][i]='\0';
    for(j=1; j<Current_Arg_Num_System; j++){
        System_command[j]=(char*)malloc(Argument_Max_Char_Input);
    }

    //copy
    k=0;
    i=1;
    j=0;
    int counter=0;
    while(UserInput_Args[Current_Comm_inline][k]!='\0'){
        while(UserInput_Args[Current_Comm_inline][k]!=' '&& UserInput_Args[Current_Comm_inline][k]!='\0'){
            System_command[i][j] = UserInput_Args[Current_Comm_inline][k];
            j++;
            k++;
        }
        if(UserInput_Args[Current_Comm_inline][k]!='\0')
            k++;
        
        System_command[i][j]='\0';
        j=0;
        i++;
        if(i==Current_Arg_Num_System){
            //expand


            System_command = (char**)realloc(System_command, sizeof(char*)*Current_Arg_Num_System*2);

            for( counter=Current_Arg_Num_System; counter<Current_Arg_Num_System*2; counter++){

                System_command[counter] = (char*)malloc(Argument_Max_Char_Input);
            }
            Current_Arg_Num_System = Current_Arg_Num_System*2;
            
        }
    }
    for(j=i; j<Current_Arg_Num_System; j++){
        temp = System_command[j];
        free(temp);
        System_command[j]= NULL;
    }
    return System_command;

}


int Execute_Command(){
    char **system_command;
    pid_t pid_redir=-2;
    int fd,redir_flag   ,pipes = 0, i  ; 

    char filepath[256];

    if (UserInput_Commands[Current_Comm_inline][COMM_MAX_CHAR_INPUT]=='|'){
        //PIPELINE
        i=Current_Comm_inline+1;
        pipes=1;
        while(UserInput_Commands[i][COMM_MAX_CHAR_INPUT]=='|'){
            pipes++;
            i++;
        }

        
        Current_Comm_inline = Current_Comm_inline + pipes;
        
        Execute_pipeline(pipes,1);
    
        return 0;
    }
    if(BuildIn_CommandHandler()!=-1){
         return 0;
    }	

    system_command = toSYSTEM_COMMAND();


    if(File_Redirection_array[Current_Comm_inline][0]!='\0'){
        redir_flag = Redirection_Handler(filepath);
        pid_redir = fork();
        if(pid_redir == -1){
            File_Redirection_array[Current_Comm_inline][0]='\0';
            printf("Fork failed\n");
            return -2;
        //=================***CHILD CODE***===========================
        }else if (pid_redir==0){//child
            if(redir_flag==0){//input
                fd = open(filepath, O_RDONLY);
            }else if(redir_flag == 1){// '>'
                fd = open(filepath,O_WRONLY | O_APPEND | O_TRUNC);
            }else{  //'>>'
                fd = open(filepath, O_WRONLY | O_APPEND);
            }
            if(fd == -1 && redir_flag > 0){ //make new file
                //file doesnt exits create a new file
                fd = open(filepath,O_CREAT | O_WRONLY , S_IRWXU);
            }else if(fd == -1 && redir_flag>0){
                printf("Couldnt open '%s' or file doesnt exist\n",filepath);
                exit(-1);
            }
            if(redir_flag == 0){//input
                dup2(fd,0);
            }else{              //output
                dup2(fd,1);
            }
        //================***CHILD CODE END============================
        
        }else{  //parent code
            wait(NULL);
            File_Redirection_array[Current_Comm_inline][0]='\0';
            free(system_command);
            return 1;
        }
    }

    pid_t pid = fork();
    
    if (pid == -1) {
		printf("\nFailed forking child..");
        free(system_command);
        if(pid_redir !=2){
            exit(-1);
        }
		return 0;
	} else if (pid == 0) {
		if (execvp(system_command[0], system_command) < 0) {
            
			printf("Could not execute command..\n");
		}
		exit(-1);
	} else {
		// waiting for child to terminate
		wait(NULL);
        free(system_command);
        if(pid_redir!=-2){
            exit(1);
        }


		return 0;
	}
}



int Redirection_Handler(char* filepath){
    int i=0;
    while(File_Redirection_array[Current_Comm_inline][i]!=' '){
        filepath[i]=File_Redirection_array[Current_Comm_inline][i];
        i++;
    }
    filepath[i]='\0';
    if(File_Redirection_array[Current_Comm_inline][i+1]=='0'){ //READ ONLY
        return 0;
    }else if(File_Redirection_array[Current_Comm_inline][i+1]=='1'){ //OVERWRITE >
        return 1;
    }
    return 2;   //WRITE AT THE END

}

//DONT TOUCH THIS
int Redirection_Handler_BuildIns(char* filepath){

    if(File_Redirection_array[Current_Comm_inline][0]=='\0'){
        return -1 ;  //ignore
    }

    int flag = Redirection_Handler(filepath);

    if(flag == 0)
        return -1; //ignore
    
    return flag;
}
