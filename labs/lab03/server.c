#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

void *handle_client(void *arg);

int main()
{
	int listenfd;
	int *connfd;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t clilen;
	pthread_t tid;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(32000);

	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 5);

	clilen = sizeof(cliaddr);
	while (1)
	{
		connfd = malloc(sizeof(int));
		*connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen);
		pthread_create(&tid, NULL, handle_client, connfd);
	}

	return 0;
}

void *handle_client(void *arg)
{
	int connfd = *((int *)arg);
	free(arg);
	char buffer[1024];
	ssize_t n;

	// Read string sent by client
	n = read(connfd, buffer, sizeof(buffer) - 1);
	if (n > 0)
	{
		buffer[n] = '\0';
		printf("Received from client: %s\n", buffer);

		// Send response to client
		char *response = "Hello from the server\n";
		write(connfd, response, strlen(response));
	}

	close(connfd);
	return NULL;
}
