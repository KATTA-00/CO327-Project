#include <pthread.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <stdio.h> 

int myglobal; 

void *thread_function(void *arg) 
{ 
	int i,j; 
	for ( i = 0; i < 5; i++ ) 
	{ 
		j = myglobal; 
		j = j + 1; 
		printf("."); 
		fflush(stdout); 
		sleep(1); 
		myglobal = j; 
	} 
	return NULL; 
} 

int main(void) 
{ 
	pthread_t mythreads[10];
	int i;
	for(i = 0; i < 10; i++)
	{ 
		if ( pthread_create(&mythreads[i], NULL, thread_function, NULL) ) 
		{ 
			printf("error creating threads."); 
			abort(); 
		}
	} 
	for(i = 0; i < 10; i++)
	{
		if ( pthread_join ( mythreads[i], NULL ) ) 
		{	 
			printf("error joining threads."); 
			abort(); 
		}
	}  
	printf("\nmyglobal equals %d\n",myglobal); 
	exit(0); 
}

