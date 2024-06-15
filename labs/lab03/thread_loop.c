#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void *thread_function(void *arg)
{
	int a;
	pthread_t thread_id = pthread_self();
	for (a = 0; a < 10; a++)
	{
		printf("Thread %lu says hi!\n", thread_id);
		sleep(2);
	}
	return NULL;
}

int main(void)
{
	pthread_t mythread[3];
	int i;

	printf("Main thread (ID: %lu) says hi!\n", pthread_self());
	for (i = 0; i < 3; i++)
	{
		if (pthread_create(&mythread[i], NULL, thread_function, NULL))
		{
			printf("Error creating thread %d.\n", i);
			abort();
		}
	}

	for (i = 0; i < 3; i++)
	{
		if (pthread_join(mythread[i], NULL))
		{
			printf("Error joining thread %d.\n", i);
			abort();
		}
	}

	exit(0);
}
