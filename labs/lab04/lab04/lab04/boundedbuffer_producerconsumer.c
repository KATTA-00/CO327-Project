#include <semaphore.h> 
#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 
#define CAPACITY 20
sem_t items; 
sem_t spaces; 
sem_t lock; 
int products = 0; 

void* consumer(void *arg) { 
	while(1) { 
		sem_wait(&items); 
		sem_wait(&lock); 
		printf("Consumer consumed %d\n",products);  
		sleep(1); 
		products--; 
		sem_post(&lock); 
		sem_post(&spaces); 
		sleep(1); 
	}   
	return NULL; 
} 

int main(int argc, char **argv) { 
	pthread_t thread; 
	if(sem_init(&items, 0, 0)) { 
		printf("Could not initialize a semaphore\n"); 
		return -1; 
	} 
	if(sem_init(&spaces, 0, CAPACITY)) { 
		printf("Could not initialize a semaphore\n"); 
		return -1; 
	} 
	if(sem_init(&lock, 0, 1)) { 
		printf("Could not initialize a semaphore\n"); 
		return -1; 
	} 
	if(pthread_create(&thread, NULL, &consumer, NULL)) { 
		printf("Could not create thread\n"); 
		return -1; 
	} 
	while(1) { 
		sem_wait(&spaces); 
		sem_wait(&lock); 
		products++; 
		printf("Producer produced %d\n",products); 
		sleep(1); 
		sem_post(&lock); 
		sem_post(&items);  
		sleep(1); 
	}	 
	return 0; 
} 

