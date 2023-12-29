#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int count(char *buffer)
{
    int count = 0;
    char *argument;
    argument = strtok(buffer, " \n");
    while (argument != NULL)
    {
        count++;
        argument = strtok(NULL, " \n");
    }
    return count;
}

void shell_boot()
{
    char buffer[512];
    char *path = "/bin/";

    while (1)
    {
        printf("myShell>");
        fgets(buffer, 512, stdin);
        int pid = fork(); // create a new process tohandle input.
        if (pid != 0)
        {
            wait(NULL);
        }
        else
        { // Child Process
            int no_of_args = count(buffer);
            char **array_of_strings = malloc((sizeof(char *) * (no_of_args + 1)));
            int i = 0;
            char *ptr;
            ptr = strtok(buffer, " \n");
            while (ptr != NULL)
            {
                array_of_strings[i] = (char *)malloc((sizeof(char) * strlen(ptr)));
                strcpy(array_of_strings[i], ptr);
                ptr = strtok(NULL, " \n");
                i++;
            }
            char *prog = malloc((sizeof(char) * (no_of_args + 1)));
            prog = strcat(strcpy(prog, path), array_of_strings[0]);

            int rv = execv(prog, array_of_strings);
        }
    }
}
int main()
{
    shell_boot();
    return 0;
}
