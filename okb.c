#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include "helpers.h"
#define READ 0
#define WRITE 1
void piper(char * plspipe) {
    char** args = parse_args(plspipe, "|");

    char** left = parse_args(args[0], " ");
    char** right = parse_args(args[1], " ");
    int fds[2];

    int fork_val = fork();
    if (fork_val == 0) {
        pipe(fds);
        fork_val = fork();
        if (fork_val) {
          close(fds[READ]);
          dup2(fds[WRITE], STDOUT_FILENO);
          execvp(left[0], left);
        } else {
          wait(NULL);
          close(fds[WRITE]);
          dup2(fds[READ], STDIN_FILENO);
          execvp(right[0], right);
        }
    } else {
        wait(NULL);
    }
}

void output_redir(char * line) {
    char ** parts = parse_args(line, ">");
    int fd = open(parts[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    char ** args = parse_args(parts[0], " ");
    execvp(args[0], args);
    close(fd);
}

void input_redir(char * line) {
    char ** parts = parse_args(line, "<");
    int fd = open(parts[1], O_RDONLY);
    dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    char ** args = parse_args(parts[0], " ");
    execvp(args[0], args);
    close(fd);
}

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
                    if(strchr(cmds[i], '>')) { //< is found
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
}
