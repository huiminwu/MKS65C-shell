#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

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

void execer(char ** args) {
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
        int i = 0;
        while (cmds[i]) {
            char ** args = parse_args(cmds[i], " ");
            if(strcmp(args[0], "cd") == 0) {
                chdir(args[1]);
            } else if(strcmp(args[0], "exit") == 0) {
                exit(0);
            } else { 
                int fork_val = fork();
                if (fork_val == 0) { //if child
                    execvp(args[0], args);
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

int main() {
    okb_looping();
	return 0;
}
