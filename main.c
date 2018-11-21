#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

char ** parse_args(char * line) {
    char ** parsed_args = calloc(5, sizeof(char **));
    char * p = line;
    int index = 0;
    while(p != NULL) {
        parsed_args[index] = strsep(&p, " ");
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

void okb_looping() {
    while(1){
        char * buff = malloc(sizeof(char) * 100);
        printf("oshkoshbogosh: \n");
        fgets(buff, 100, stdin);
        
        char * c = buff;
        while(*c != '\n' && *c) {
            c++;
        }
        *c = '\0';

        char ** args = parse_args(buff);
        //print_arr(args);
        int fork_val = fork();
        if (fork_val == 0) { //if child
            execvp(args[0], args);
            exit(0);
        }

        if (fork_val != 0) { //if parent
            int status;
            wait(&status);
            continue;
        }
        free(buff);
    }
}

int main() {
    okb_looping();
	return 0;
}
