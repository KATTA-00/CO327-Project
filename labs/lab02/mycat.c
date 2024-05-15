#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char *argv[])
{
    int file;          // File descriptor
    char buffer[SIZE]; // Buffer for reading data
    int bytes_read;    // Number of bytes read

    // Check if the correct number of arguments is provided
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file in read-only mode
    file = open(argv[1], O_RDONLY);
    if (file == -1)
    {
        fprintf(stderr, "Couldn't open the file for reading\n");
        return 1;
    }

    // Read from the file and write to stdout
    while ((bytes_read = read(file, buffer, SIZE)) > 0)
    {
        if (write(STDOUT_FILENO, buffer, bytes_read) != bytes_read)
        {
            fprintf(stderr, "Reading Error\n");
            close(file);
            return 1;
        }
    }

    // Check for read error
    if (bytes_read == -1)
    {
        fprintf(stderr, "Reading Error\n");
        close(file);
        return 1;
    }

    // Close the file
    if (close(file) == -1)
    {
        fprintf(stderr, "Closing Error\n");
        return 1;
    }

    return 0;
}
