#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Include for wait function

int main(void)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        exit(1);
    }
    else if (pid == 0)
    {
        // This is the child process
        printf("This is the child process (PID: %d)\n", getpid());
    }
    else
    {
        // This is the parent process
        wait(NULL); // Wait for child process to finish
        printf("This is the parent process (PID: %d)\n", getpid());
    }

    return 0;
}
