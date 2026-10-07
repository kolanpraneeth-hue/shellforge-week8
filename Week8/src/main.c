
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "input.h"
#include "parser.h"
#include "process.h"
#include "builtin.h"
#include "signals.h"
#include "pipes.h"

static void tokenize(char *str, char **argv)
{
    int i = 0;

    char *token = strtok(str, " \t\n");

    while (token != NULL) {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }

    argv[i] = NULL;
}

int main()
{
    char *line;
    char **tokens;

    initialize_signals();

    while (1) {

        printf("myshell> ");

        line = read_line();

        /* Ctrl+D */
        if (line == NULL) {
            break;
        }

        /* Skip empty lines */
        if (line[0] == '\0') {
            free(line);
            continue;
        }

        /* Pipe command */
        if (strchr(line, '|') != NULL) {

            char *argv1[64];
            char *argv2[64];

            char *left = strtok(line, "|");
            char *right = strtok(NULL, "|");

            if (left == NULL || right == NULL) {
                printf("Invalid pipe command\n");
                free(line);
                continue;
            }

            /* Trim leading spaces */
            while (*left == ' ')
                left++;

            while (*right == ' ')
                right++;

            tokenize(left, argv1);
            tokenize(right, argv2);

            if (argv1[0] == NULL || argv2[0] == NULL) {
                printf("Invalid pipe command\n");
                free(line);
                continue;
            }

            execute_pipe(argv1, argv2);

            free(line);
        }

        /* Normal command */
        else {

            /* Handle exit */
            if (strcmp(line, "exit") == 0) {
                free(line);
                break;
            }

            tokens = parse_line(line);

            /* Check built-in commands first */
            if (execute_builtin(tokens) == 0) {
                execute(tokens);
            }

            free_tokens(tokens);
            free(line);
        }
    }

    return 0;
}



