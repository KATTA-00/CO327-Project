#include <semaphore.h> 
#include <pthread.h> 
#include <stdio.h> 
#include <unistd.h> 

#define CAPACITY 20

sem_t items; 
sem_t spaces; 
sem_t lock; 
int products = 0;
struct drand48_data
  {
    unsigned short int __x[3];	/* Current state.  */
    unsigned short int __old_x[3]; /* Old state.  */
    unsigned short int __c;	/* Additive const. in congruential formula.  */
    unsigned short int __init;	/* Flag for initializing.  */
    unsigned long long int __a;	/* Factor in congruential formula.  */
  };
 
struct drand48_data randBuffer;

void* consumer(void *arg) {
	double x; 
	srand48_r(time(NULL), &randBuffer);
	while(1) { 
		sem_wait(&items); 
		sem_wait(&lock); 
		printf("Consumer consumed %d\n",products);  
		drand48_r(&randBuffer, &x);	
		usleep(x*100);
		products--; 
		sem_post(&lock); 
		sem_post(&spaces); 
		drand48_r(&randBuffer, &x);	
		usleep(x*100);
	}   
	return NULL; 
} 

int main(int argc, char **argv) { 
	pthread_t thread;
	double y; 
	srand48_r(time(NULL), &randBuffer);

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
		drand48_r(&randBuffer, &y);	
		usleep(y*100);
		sem_post(&lock); 
		sem_post(&items);  
		drand48_r(&randBuffer, &y);	
		usleep(y*100);
	}	 
	return 0; 
} 


