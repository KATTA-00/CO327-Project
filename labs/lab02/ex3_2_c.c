#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define INPUTFILE "fixtures"

/**
 * Executes the command "cat fixtures | grep <search_term> | cut -b 1-9".
 */

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
        return 1;
    }

    int status;
    int pipes[4];

    // arguments for commands
    char *cat_args[] = {"cat", INPUTFILE, NULL};
    char *grep_args[] = {"grep", "-i", argv[1], NULL};
    char *cut_args[] = {"cut", "-b", "1-9", NULL};

    // set up first pipe (cat to grep)
    if (pipe(pipes) == -1)
    {
        perror("pipe");
        return 1;
    }

    // set up second pipe (grep to cut)
    if (pipe(pipes + 2) == -1)
    {
        perror("pipe");
        return 1;
    }

    // fork the first child (to execute cat)
    if (fork() == 0)
    {
        // replace cat's stdout with write part of 1st pipe
        dup2(pipes[1], 1);

        // close all pipes
        close(pipes[0]);
        close(pipes[1]);
        close(pipes[2]);
        close(pipes[3]);

        // execute cat command
        execvp("cat", cat_args);
        perror("execvp cat");
        return 1;
    }
    else
    {
        // fork second child (to execute grep)
        if (fork() == 0)
        {
            // replace grep's stdin with read end of 1st pipe
            dup2(pipes[0], 0);

            // replace grep's stdout with write end of 2nd pipe
            dup2(pipes[3], 1);

            // close all pipes
            close(pipes[0]);
            close(pipes[1]);
            close(pipes[2]);
            close(pipes[3]);

            // execute grep command
            execvp("grep", grep_args);
            perror("execvp grep");
            return 1;
        }
        else
        {
            // fork third child (to execute cut)
            if (fork() == 0)
            {
                // replace cut's stdin with read end of 2nd pipe
                dup2(pipes[2], 0);

                // close all pipes
                close(pipes[0]);
                close(pipes[1]);
                close(pipes[2]);
                close(pipes[3]);

                // execute cut command
                execvp("cut", cut_args);
                perror("execvp cut");
                return 1;
            }
        }
    }

    // only the parent gets here and waits for 3 children to finish
    // close all pipes in the parent
    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);

    // wait for children
    for (int i = 0; i < 3; i++)
    {
        wait(&status);
    }

    return 0;
}
