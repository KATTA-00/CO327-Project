#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 100

int main(int argc, char *argv[])
{
    int fileFrom, fileTO; // File descriptor
    char buffer[SIZE];    // Buffer for reading data
    int bytes_read;       // Number of bytes read

    // Check if the correct number of arguments is provided
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    // Open the file in read-only mode
    fileFrom = open(argv[1], O_RDONLY);
    fileTO = open(argv[2], O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    if (fileFrom == -1 || fileTO == -1)
    {
        fprintf(stderr, "Couldn't open the file for reading\n");
        return 1;
    }

    // Read from the file and write to stdout
    while ((bytes_read = read(fileFrom, buffer, SIZE)) > 0)
    {
        if (write(fileTO, buffer, bytes_read) != bytes_read)
        {
            fprintf(stderr, "Reading Error\n");
            close(fileFrom);
            close(fileTO);
            return 1;
        }
    }

    // Check for read error
    if (bytes_read == -1)
    {
        fprintf(stderr, "Reading Error\n");
        close(fileFrom);
        close(fileTO);
        return 1;
    }

    // Close the file
    close(fileFrom);
    close(fileTO);

    return 0;
}
