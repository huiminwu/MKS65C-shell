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
