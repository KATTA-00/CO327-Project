#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    // Check if an argument is provided
    if (argc < 2)
    {
        printf("Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    // Execute ls command with -l option and the provided directory
    execl("/bin/ls", "ls", "-l", argv[1], NULL);

    // This line will only be reached if execl fails
    // It indicates that the ls program has terminated
    puts("Program ls has terminated");

    return 0;
}
