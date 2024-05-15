#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 256

int main()
{
    int pipe_ends_child2parent[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    int count;

    // Create pipe
    if (pipe(pipe_ends_child2parent))
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
        close(pipe_ends_child2parent[WRITE_END]); // Close unused write end

        // Wait for child process to finish
        wait(NULL);

        // Read output from child process
        printf("The Output from child:\n");

        count = read(pipe_ends_child2parent[READ_END], buffer, BUFFER_SIZE);

        // Write output to stdout
        write(STDOUT_FILENO, buffer, count);

        close(pipe_ends_child2parent[READ_END]); // Close read end
        exit(EXIT_SUCCESS);                      // Exit parent process
    }

    if (pid == 0)
    { // Child process
        // Redirect stdout to the write end of the pipe
        dup2(pipe_ends_child2parent[WRITE_END], STDOUT_FILENO);

        // Close unused ends of the pipe
        close(pipe_ends_child2parent[READ_END]);
        close(pipe_ends_child2parent[WRITE_END]);

        // Execute ls command
        if (execlp("ls", "ls", NULL) == -1)
        {
            perror("execlp failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
