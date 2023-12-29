#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFF_SIZE 1024
#define TK_DELIMETER " \t\r\n"
#define TK_BUFF_SIZE 64

char **parse_cmd(char *cmd)
{
    int buffsize = TK_BUFF_SIZE;
    int ptr = 0;
    char *token;
    char **tokens = malloc(buffsize * sizeof(char *)); // Array that stores tokens.

    if (!tokens)
    {
        fprintf(stderr, "Failed");
        exit(EXIT_FAILURE);
    }

    token = strtok(cmd, TK_DELIMETER);
    while (token != NULL)
    {
        tokens[ptr] = token;
        ptr++;

        if (ptr >= buffsize)
        {
            buffsize += TK_BUFF_SIZE;
            tokens = realloc(tokens, buffsize * sizeof(char *));

            if (!tokens)
            {
                fprintf(stderr, "Memory Allocation Failure.\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, TK_DELIMETER);
    }

    tokens[ptr] = NULL;

    return tokens;
}

int main()
{
    char *cmd;
    char **args;
    int status = 1;
    while (1)
    {
        printf("DC126732_Shell $ ");
        cmd = scanf(&cmd);
        args = parse_cmd(cmd);
        int pid = fork();
        if (pid != 0) // Parent Process
        {
            wait(NULL);
        }
        else if (pid == 0) // Child process
        {

            free(cmd);
            free(args);
        }
        else
        {
            printf("Failed Creating Process.\n");
            exit(EXIT_FAILURE);
        }
    }
}