#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FIFO1 "/tmp/fifo1"
#define FIFO2 "/tmp/fifo2"
#define MAX_SIZE 1024

int main()
{
    int fd1, fd2;
    int len;
    char input[MAX_SIZE];
    char output[MAX_SIZE];

    while (1)
    {
        // Get input from the user
        printf("Enter a string: ");
        fgets(input, MAX_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0'; // Remove the newline character
        len = strlen(input);

        // Create the FIFOs
        mkfifo(FIFO1, 0666);
        // Send the input to the capitalizer program
        fd1 = open(FIFO1, O_WRONLY);
        write(fd1, input, strlen(input) + 1);
        close(fd1);

        // Create the FIFOs
        mkfifo(FIFO2, 0666);
        // Receive the capitalized string from the capitalizer program
        fd2 = open(FIFO2, O_RDONLY);
        read(fd2, output, MAX_SIZE);
        close(fd2);

        output[len] = '\0'; // Add the null terminator

        // Print the capitalized string
        printf("Capitalized string: %s\n", output);

        // Remove the FIFOs
        unlink(FIFO1);
        unlink(FIFO2);
    }

    return 0;
}
