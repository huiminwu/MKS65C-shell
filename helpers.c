#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#define READ 0
#define WRITE 1

/*****************************************************************
 * Separates parts of a line with delimiter
 *
 * Arguments:
 *  char * line
 *      the line you want to separate
 *  char * delimiter
 *      the character you want to separate line with
 * 
 * Returns:
 *  char **
 *      an array of strings after separating the line
 * ***************************************************************/

char ** parse_args(char * line, char * delimiter) {
    char ** parsed_args = malloc(256*sizeof(char *));
    char * p = line;
    int index = 0;
    while(p != NULL) {
        parsed_args[index] = strsep(&p, delimiter);
        char * arg = parsed_args[index];
        //strip leading whitespace
        while(*arg && isspace(*arg)) {
            arg++;
        }
        int end = strlen(arg) - 1;
        while(arg[end] == ' ' ){
            end--;
        }
        arg[end+1] = 0; //add an end to the phrase
        parsed_args[index] = arg;
        index++; 
    }
    
    return parsed_args;
}

/*****************************************************************
 * Prints the contents of an array (mainly used for debugging)
 *
 * Arguments:
 *  char ** args
 *      the array you want to print
 * Returns:
 *  nothing because its a print function
 * ***************************************************************/

void print_arr(char ** args) {
    int i = 0;
    while (args[i]) {
        printf("thing * index %i : %s\n", i, args[i]);
        i++;
    }
}

/*****************************************************************
 * Handles commands with |. Redirects stdout from one program to
 * stdin of the next
 *
 * Arguments:
 *  char * plspipe
 *      the command with | that will be run
 * Returns:
 *  nothing
 * ***************************************************************/
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

/*****************************************************************
 * Handles commands with >. Redirects stdout to a file. If the file
 * already has contents, those contents are erased.
 *
 * Arguments:
 *  char * line
 *      the command with > that will be run
 * Returns:
 *  nothing
 * ***************************************************************/
void output_redir(char * line) {
    char ** parts = parse_args(line, ">");
    int fd = open(parts[1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    dup(STDOUT_FILENO);
    dup2(fd, STDOUT_FILENO);
    char ** args = parse_args(parts[0], " ");
    execvp(args[0], args);
    close(fd);
}

/*****************************************************************
 * Handles commands with <. Executes command with redirected stdin from file.
 *
 * Arguments:
 *  char * line
 *      the command with < that will be run
 * Returns:
 *  nothing
 * ***************************************************************/
void input_redir(char * line) {
    char ** parts = parse_args(line, "<");
    int fd = open(parts[1], O_RDONLY);
    dup(STDIN_FILENO);
    dup2(fd, STDIN_FILENO);
    char ** args = parse_args(parts[0], " ");
    execvp(args[0], args);
    close(fd);
}
