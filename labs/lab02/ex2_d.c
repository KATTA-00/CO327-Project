#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 256

void capitalize(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (isalpha(str[i]))
        {                             // Check if character is alphabetic
            str[i] = toupper(str[i]); // Convert to uppercase
        }
    }
}

int main()
{
    int pipe_ends_parent2child[2], pipe_ends_child2parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    int count;

    // Create pipes
    if (pipe(pipe_ends_parent2child) || pipe(pipe_ends_child2parent))
    {
        perror("Pipe creation");
        return -1;
    }

    // Fork process
    pid = fork();
    if (pid < 0)
    {
        perror("Fork");
        return -1;
    }

    if (pid > 0)
    {                                             // Parent process
        close(pipe_ends_parent2child[READ_END]);  // Close unused read end
        close(pipe_ends_child2parent[WRITE_END]); // Close unused write end

        printf("Enter a line of text: ");

        // Read input from user
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL)
        {
            printf("Error reading input.\n");
        }

        // Write input to child process
        write(pipe_ends_parent2child[WRITE_END], buffer, strlen(buffer));
        close(pipe_ends_parent2child[WRITE_END]);

        // Wait for child process to finish
        wait(NULL);

        // Read output from child process
        count = read(pipe_ends_child2parent[READ_END], buffer, BUFFER_SIZE);
        close(pipe_ends_child2parent[READ_END]);

        printf("The Output from child: %s", buffer);

        exit(EXIT_SUCCESS); // Exit parent process
    }

    if (pid == 0)
    {                                             // Child process
        close(pipe_ends_parent2child[WRITE_END]); // Close unused write end
        close(pipe_ends_child2parent[READ_END]);  // Close unused read end

        // Read input from parent process
        count = read(pipe_ends_parent2child[READ_END], buffer, BUFFER_SIZE);
        close(pipe_ends_parent2child[READ_END]); // Close unused read end

        // Capitalize alphabetic characters
        capitalize(buffer);

        // Write modified input back to parent process
        write(pipe_ends_child2parent[WRITE_END], buffer, strlen(buffer));
        close(pipe_ends_child2parent[WRITE_END]); // Close unused write end
    }

    return 0;
}
