#include "tsh.h"
#include <unistd.h>

using namespace std;

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
    // TODO: tokenize the command string into arguments
    char *buf = (char *)malloc(25 * sizeof(char));
    int bufIndex = 0;
    int tokIndex = 0;

    for(int i = 0; i < 81; i++){
        if(cmd[i] == ' '){
            cmdTokens[tokIndex] = buf;
            buf = (char *)malloc(25 * sizeof(char));
            tokIndex++;
            bufIndex = 0;
        }else if(cmd[i] == '\n'){
            break;
        }else{
            buf[bufIndex] = cmd[i];
            bufIndex++;
        }
    }
    cmdTokens[tokIndex] = buf;
    cmdTokens[tokIndex+1] = (char *) NULL;
}

void simple_shell::exec_command(char **argv){
    // TODO: fork a child process to execute the command.
    // parent process should wait for the child process to complete and reap it
    pid_t pid = fork();
    if(pid < 0){
        fprintf(stderr, "fork failed\n");
        exit(1);
    }else if(pid == 0){
        char *cmd = *argv;
        *argv = "";
        int result = execvp(cmd, argv);
        if(result == -1){
            exit(0);
        }
    }else{
        int wc = waitpid(pid, NULL, NULL);
    }
}

bool simple_shell::isQuit(char *cmd){
    // TODO: check for the command "quit" that terminates the shell
    return (strcmp(cmd, "quit") == 0);
}
