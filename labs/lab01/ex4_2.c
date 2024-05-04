#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h> // Include for wait function
#include <stdlib.h>

#define MAX_STRING_LENGTH 100
#define MAX_TOKENS 100

int main(int argc, char **argv)
{

    char str[MAX_STRING_LENGTH];
    char *tokens[MAX_TOKENS];
    char *token;
    pid_t pid;

    printf("Welcome to MY_SHELL\n");

    while (1)
    {
        printf(">> ");
        fgets(str, MAX_STRING_LENGTH, stdin);

        // Removing the trailing newline character if present
        if (str[strlen(str) - 1] == '\n')
        {
            str[strlen(str) - 1] = '\0';
        }

        if (str[0] == 'q' && strlen(str) == 1)
            break;

        // Get the first token
        token = strtok(str, " ");

        int count = 0;
        while (token != NULL && count < MAX_TOKENS - 1)
        {
            tokens[count++] = token;
            token = strtok(NULL, " ");
        }
        tokens[count] = NULL; // Null-terminate the array of tokens

        // Append NULL to the end of the tokens array
        tokens[count] = NULL;

        pid = fork();

        if (pid < 0)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        {
            // Execute the command using execvp
            execvp(tokens[0], tokens);
            perror("execvp");
            exit(1);
        }
        else
        {
            // This is the parent process
            wait(NULL); // Wait for child process to finish
        }
    }

    // end of program
    puts("Program has terminated");

    return 0;
}
