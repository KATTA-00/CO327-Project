#include <semaphore.h> 
#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 

sem_t items; 

void* consumer(void *arg) { 
	while(1) { 
		sem_wait(&items);  
		printf("Consumer consumed\n"); 
		sleep(1); 
	}   
	return NULL; 
} 

int main(int argc, char **argv) { 
	pthread_t thread; 
	if(sem_init(&items, 0, 0))	{ 
		printf("Could not initialize a semaphore\n"); 
		return -1; 
	} 
	if(pthread_create(&thread, NULL, &consumer, NULL)) { 
		printf("Could not create thread\n"); 
		return -1; 
	} 
	while(1) { 
		printf("Producer produced\n");
		sleep(1); 
		sem_post(&items);  
	}	 
	return 0; 
} 

