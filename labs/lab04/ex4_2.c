#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 10
#define NUM_READERS 10
#define NUM_WRITERS 10

int array[ARRAY_SIZE];             // Array to store values
sem_t readers[ARRAY_SIZE];         // Semaphores for controlling access by readers
sem_t writers[ARRAY_SIZE];         // Semaphores for controlling access by writers
sem_t readerCountMutex;            // Mutex for protecting reader count
int readerCount[ARRAY_SIZE] = {0}; // Track the number of readers reading each index

int generateRandomIndex()
{
    return rand() % ARRAY_SIZE; // Generate random index within array bounds
}

void *reader(void *arg)
{
    while (1)
    {
        int index = generateRandomIndex();

        sem_wait(&readers[index]);   // Wait for permission to read
        sem_wait(&readerCountMutex); // Lock to update reader count
        readerCount[index]++;        // Increment reader count
        if (readerCount[index] == 1) // First reader for this index
        {
            sem_wait(&writers[index]); // Wait for writers to finish
        }
        sem_post(&readerCountMutex); // Unlock

        sem_post(&readers[index]); // Release semaphore

        int value = array[index]; // Read value
        printf("Reader read value %d at index %d\n", value, index);

        sem_wait(&readerCountMutex); // Lock to update reader count
        readerCount[index]--;        // Decrement reader count
        if (readerCount[index] == 0) // Last reader for this index
        {
            sem_post(&writers[index]); // Allow writers to proceed
        }
        sem_post(&readerCountMutex); // Unlock

        usleep(rand() % 1000000); // Sleep for random interval
    }

    return NULL;
}

void *writer(void *arg)
{
    while (1)
    {
        int index = generateRandomIndex();

        sem_wait(&writers[index]); // Wait for permission to write
        array[index]++;            // Increment value
        printf("Writer incremented value at index %d to %d\n", index, array[index]);
        sem_post(&writers[index]); // Release semaphore

        usleep(rand() % 1000000); // Sleep for random interval
    }

    return NULL;
}

int main()
{
    // Initialize array values
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = 0;
    }

    // Initialize semaphores and mutex
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sem_init(&readers[i], 0, 1);
        sem_init(&writers[i], 0, 1);
    }
    sem_init(&readerCountMutex, 0, 1);

    pthread_t readerThreads[NUM_READERS]; // Reader threads
    pthread_t writerThreads[NUM_WRITERS]; // Writer threads

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++)
    {
        pthread_create(&readerThreads[i], NULL, reader, NULL);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        pthread_create(&writerThreads[i], NULL, writer, NULL);
    }

    // Wait for all reader threads to finish
    for (int i = 0; i < NUM_READERS; i++)
    {
        pthread_join(readerThreads[i], NULL);
    }

    // Wait for all writer threads to finish
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        pthread_join(writerThreads[i], NULL);
    }

    // Destroy semaphores and mutex
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        sem_destroy(&readers[i]);
        sem_destroy(&writers[i]);
    }
    sem_destroy(&readerCountMutex);

    return 0;
}