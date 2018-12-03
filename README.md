# MKS65C-shell
By: Hui Min Wu Pd 10 Systems

## Features shell implements
* Read, separate, and execute multiple commands in one line with ;
* Can run cd and exit
* Input and output redirection with > and <
* Single pipe

## Description of what features I attempted but were unsuccessful

## Bugs
* grep function doesn't work
* In rare cases, you have to type "exit" a second time to exit
* Does not correct whitespace
* Must have no spaces before or after ">", "<", or "|"
* Does not handle commands with more than 100 characters

## Function Headers
### Headers in helpers.h

```char ** parse_args(char * line, char *delimiter);```
* Separates parts of a line with delimiter
* Arguments:
  * char * line
    * the line you want to separate
  * char * delimiter
    * the character you want to separate line with
* Returns:
  * char **
    *an array of strings after separating the line
___

```void print_arr(char ** args);```
* Prints the contents of an array (mainly used for debugging)
* Arguments:
  * char ** args
    * the array you want to print
* Returns:
  * nothing because its a print function
___
```void piper(char * plspipe);```
* Handles commands with |. Redirects stdout from one program to stdin of the next

* Arguments:
  * char * plspipe
    * the command with | that will be run
* Returns:
  * nothing
___
```void output_redir(char * line);```
* Handles commands with >. Redirects stdout to a file. If the file already has contents, those contents are erased.
* Arguments:
  * char * line
    * the command with > that will be run
* Returns:
  * nothing
___
```void input_redir(char * line);```
* Handles commands with <. Executes command with redirected stdin from file.
* Arguments:
  * char * line
    * the command with < that will be run
* Returns:
  * nothing
### Headers in okb.h

```int main();```
* Prompts user for commands and executes them accordingly.
* Arguments:
  * None
* Return Value: 
  * integer, to comply with standard convention of a main function
