#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARG 4
#define MAX_LINES 1024

void parse_arguments(char *input, char **args){
    // Tokenize input based on space and newlines
    int i = 0;
    args[i] = strtok(input, " \n"); // Get first token

    // Get rest of tokens until NULL or reached max arguments
    while (args[i] != NULL && i < MAX_ARG){
        i++;
        args[i] = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

int main(){
    char input[MAX_LINES];
    char *args[MAX_ARG];
    pid_t pid;

    while(1){
        printf("myShell ==> ");
        fflush(stdout);

        // Read input
        fgets(input, MAX_LINES, stdin);
        input[strcspn(input, "\n")] = '\0'; //Clear newlines

        // Exit if only exit
        if(strcmp(input, "exit") == 0){
            break;
        }

        // Parse input into arguments
        parse_arguments(input, args);

        // Limit ping to -c 4; Move [1] to [3]
        // if(strcmp(args[0], "ping") == 0){
        //     args[3] = args[1];
        //     args[1] = "-c";
        //     args[2] = "4";
        // }

        // Forking
        pid = fork();

        if (pid < 0){
            perror("Fork Failed");
            exit(1);
        }
        else if (pid == 0){
            // The child process
            if(execvp(args[0], args) == -1){
                perror("Exec Failed");
                exit(1);
            }
        }
        else{
            // Wait for completion
            wait(NULL);
        }
    }

    printf("Thanks for using myShell!");
    return 0;
}