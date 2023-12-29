#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFF_SIZE 1024
#define TK_DELIMETER " \n"
#define TK_BUFF_SIZE 64

// Read the user input and then stores it into the buffer.
char *read_cmd()
{
    int buffsize = BUFF_SIZE;
    int ptr = 0;
    char *buffer = malloc(sizeof(char) * buffsize); // Dynamic Memory Allocation to handle unpredictable user inputs.
    int c;                                          // EOF is an integer.

    while (1)
    {
        c = getchar();
        if (c == '\0' || c == '\n') // End of File or Next Line, end of command.
        {
            buffer[ptr] = '\0';
            return buffer;
        }
        else
        {
            buffer[ptr] = c; // Insert character into the buffer.
        }
        ptr++;

        if (ptr >= buffsize) // Resize the buffer in case the user command outranges.
        {
            buffsize += 1024;
            buffer = realloc(buffer, buffsize);
        }
    }
}

// Parse the user input into tokens.
char **parse_cmd(char *cmd)
{
    int buffsize = TK_BUFF_SIZE;
    int ptr = 0;
    char *token;                                       // A token, a char_array pointer.
    char **tokens = malloc(buffsize * sizeof(char *)); // Array that stores tokens.

    token = strtok(cmd, TK_DELIMETER); // Pase using delimeter " " and "\n".

    while (token != NULL)
    {
        tokens[ptr] = token; // Place the parsed token into the token array.
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

// Execut tokens.
int shell_execute(char **args) // Pass a token array into the function.
{
    int status;

    if (strcmp(args[0], "exit") == 0) // special "exit" command
    {
        return 0;
    }

    int child_pid = fork(); // Create a Child Process

    if (child_pid == 0) // Child Process
    {
        execvp(args[0], args); // Execute command tokens.
        printf("Command not defined: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }
    else if (child_pid < 0) // Failed Creation
        printf("Error: Failed creating process.");
    else // Parent Process
    {
        wait(&status);
    }
    return 1;
}

int main()
{
    char *cmd;   // User input command
    char **args; // Parsed token array from the command.
    int status = 1;

    while (status)
    {
        printf("DC126732_Shell $ ");
        cmd = read_cmd();
        args = parse_cmd(cmd);
        status = shell_execute(args);
        free(cmd);
        free(args);
    };
}