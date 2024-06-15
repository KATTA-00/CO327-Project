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
    char input[MAX_SIZE];
    char output[MAX_SIZE];

    while (1)
    { // Open the FIFO for reading the input
        mkfifo(FIFO1, 0666);
        fd1 = open(FIFO1, O_RDONLY);
        read(fd1, input, MAX_SIZE);
        close(fd1);

        // Open the FIFO for writing the output
        mkfifo(FIFO2, 0666);
        fd2 = open(FIFO2, O_WRONLY);

        // replace standard output with output file
        dup2(fd2, 1);

        // Capitalize the input string using the tr command
        // Send the capitalized string back to the writer program
        FILE *tr_pipe = popen("tr '[a-z]' '[A-Z]'", "w");
        fwrite(input, sizeof(char), strlen(input), tr_pipe);
        pclose(tr_pipe);

        close(fd2);
    }

    return 0;
}
