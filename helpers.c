#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#define READ 0
#define WRITE 1

char ** parse_args(char * line, char * delimiter) {
    char ** parsed_args = calloc(5, sizeof(char **));
    char * p = line;
    int index = 0;
    while(p != NULL) {
        parsed_args[index] = strsep(&p, delimiter);
        index++; 
    }
    return parsed_args;
}

void print_arr(char ** args) {
    int i = 0;
    while (args[i]) {
        printf("thing * index %i : %s\n", i, args[i]);
        i++;
    }
}

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
