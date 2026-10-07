#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "input.h"
#include "process.h"
#include "builtin.h"

#define MAX_TOKENS 64

char **parse_line(char *line)
{
    int bufsize = MAX_TOKENS;
    int position = 0;

    char **tokens = malloc(bufsize * sizeof(char *));

    if (tokens == NULL)
    {
        perror("ShellForge");
        exit(EXIT_FAILURE);
    }

    char *token = strtok(line, " \t");

    while (token != NULL)
    {
        tokens[position++] = token;

        if (position >= bufsize - 1)
        {
            bufsize *= 2;

            tokens = realloc(tokens, bufsize * sizeof(char *));

            if (tokens == NULL)
            {
                perror("ShellForge");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t");
    }

    tokens[position] = NULL;

    return tokens;
}

int main(void)
{
    char *line;
    char **tokens;

    printf("=====================================\n");
    printf(" Welcome to ShellForge Version 4.0\n");
    printf("=====================================\n");

    while (1)
    {
        printf("myshell> ");

        line = read_line();

        if (line == NULL)
            break;

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            printf("Exiting ShellForge...\n");
            break;
        }

        tokens = parse_line(line);

        if (tokens[0] != NULL)
        {
            if (execute_builtin(tokens) == 0)
            {
                execute(tokens);
            }
        }

        free(tokens);
        free(line);
    }

    return 0;
}
