#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "helpers.h"

/******************************************************************
 * Prompts user for commands and executes them accordingly.
 * 
 * Arguments:
 *  None
 *
 * Return Value: 
 *  integer, to comply with standard convention of a main function
 * 
 ******************************************************************/
int main() {
    while(1){
        char * buff = malloc(sizeof(char) * 100);
        char hostname[256];
        gethostname(hostname, 255);
        printf("%s: ", hostname);
        fgets(buff, 100, stdin);

        char * c = buff;
        while(*c != '\n' && *c) {
            c++;
        }
        *c = '\0';
        char ** cmds = parse_args(buff, ";");
        char ** args;
        int i = 0;

        while (cmds[i]) {
            char * cmd_cop = malloc(sizeof(char *));
            strcpy(cmd_cop, cmds[i]);
            args = parse_args(cmd_cop, " ");

            if(strcmp(args[0], "cd") == 0) {
                chdir(args[1]);
            } else if(strcmp(args[0], "exit") == 0) {
                exit(0);
            } else if (strchr(cmds[i], '|')) {
                piper(cmds[i]);
            } else { 
                int fork_val = fork();
                if (fork_val == 0) { //if child
                    if(strchr(cmds[i], '>')) {
                        output_redir(cmds[i]);
                    } else if (strchr(cmds[i], '<')) {
                        input_redir(cmds[i]);
                    } else {
                        args = parse_args(cmds[i], " ");
                        execvp(args[0], args);
                    }
                }

                if (fork_val != 0) { //if parent
                    int status;
                    wait(&status);
                    i++;
                    continue;
                }
            }
            i++;
        }
        free(buff);
    }
    return 0;
}
