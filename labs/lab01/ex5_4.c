#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Sample TCP server */
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

int sockfd, newsockfd, n;
struct sockaddr_in servaddr, cli_addr;
socklen_t clilen;

#define CHUNK_SIZE 1000

int handle_client(int connfd)
{
    char buffer[1000];
    FILE *file;

    // Receive data from the client and store it in buffer
    n = recvfrom(connfd, buffer, 1000, 0, (struct sockaddr *)&cli_addr, &clilen);

    // Add null terminator to make it a string
    buffer[n - 1] = '\0';

    printf("Received: %s\n", buffer);

    // Open the file for reading
    file = fopen(buffer, "r");

    if (file == NULL)
    {
        perror("Error opening file");
        // Send an error message to the client
        sendto(connfd, "Error opening file", strlen("Error opening file"), 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
        return 1;
    }

    // Read the file in chunks of 1000 bytes
    while (!feof(file))
    {
        size_t bytesRead = fread(buffer, 1, CHUNK_SIZE, file);

        // Add a null terminator to make it a valid C string
        buffer[bytesRead] = '\0';

        // Send the chunk of data to the client
        sendto(connfd, buffer, bytesRead, 0, (struct sockaddr *)&cli_addr, sizeof(cli_addr));
    }

    // Close the file if it was successfully opened
    fclose(file);

    // Print the received message

    return 0;
}

int main(void)
{
    pid_t pid;

    /* Create a socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /* Initialize server address struct */
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Listen on any interface
    servaddr.sin_port = htons(12345);             // Listen on port 12345

    /* Bind the socket to the server address */
    bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

    /* Listen for incoming connections */
    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    while (1)
    {
        /* Accept a new connection */
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        /* Fork a child process to handle the connection */
        pid = fork();

        if (pid < 0)
        {
            perror("ERROR on fork");
            exit(1);
        }

        if (pid == 0)
        {
            /* In child process which handles the client connection */
            close(sockfd);            // Close the listening socket in the child process
            handle_client(newsockfd); // Handle client communication
            exit(0);                  // Exit the child process
        }
        else
        {                     /* In parent process which continues to listen for new clients */
            close(newsockfd); // Close the new socket in the parent process
        }
    }

    return 0;
}
