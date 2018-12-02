#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "helpers.h"
void okb_looping() {
    while(1){
        char * buff = malloc(sizeof(char) * 100);
        printf("oshkoshbogosh: ");
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
            if(strcmp(cmds[0], "cd") == 0) {
                chdir(cmds[1]);
            } else if(strcmp(cmds[0], "exit") == 0) {
                exit(0);
            } else { 
                int fork_val = fork();
                if (fork_val == 0) { //if child
                    if(strchr(cmds[i], '>')) { //< is found
                        char ** parts = parse_args(cmds[i], ">");
                        int fd = open(parts[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
                        dup2(fd, STDOUT_FILENO);
                        args = parse_args(parts[0], " ");
                        execvp(args[0], args);
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
}
