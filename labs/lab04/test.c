#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ARRAY_SIZE 10
#define NUM_READERS 10
#define NUM_WRITERS 10

int array[ARRAY_SIZE];
sem_t readers[ARRAY_SIZE]; // Semaphore for controlling access by readers
sem_t writers[ARRAY_SIZE]; // Semaphore for controlling access by writers

// Function to generate a random integer between 0 and ARRAY_SIZE-1
int generateRandomIndex()
{
    return rand() % ARRAY_SIZE;
}

void *reader(void *arg)
{
    while (1)
    {
        int index = generateRandomIndex();

        sem_wait(&readers[index]); // Wait for permission to read

        // Wait until the writer for this index is available
        sem_wait(&writers[index]);

        int value = array[index];
        printf("Reader read value %d at index %d\n", value, index);

        sem_post(&writers[index]); // Release writer semaphore
        sem_post(&readers[index]); // Signal that a reader has finished reading

        usleep(rand() % 1000000); // Sleep for a random interval
    }

    return NULL;
}

void *writer(void *arg)
{
    while (1)
    {
        int index = generateRandomIndex();

        sem_wait(&writers[index]); // Wait for permission to write

        array[index]++;
        printf("Writer incremented value at index %d to %d\n", index, array[index]);

        sem_post(&writers[index]); // Signal that a writer has finished writing

        usleep(rand() % 1000000); // Sleep for a random interval
    }

    return NULL;
}

int main()
{
    // initailize the array
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = 0;
    }

    pthread_t readerThreads[NUM_READERS];
    pthread_t writerThreads[NUM_WRITERS];

    // Initialize semaphores
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (sem_init(&readers[i], 0, 1)) // Initialize reader semaphore with 1 to allow multiple readers
        {
            printf("Could not initialize the readers semaphore\n");
            return -1;
        }

        if (sem_init(&writers[i], 0, 1))
        {
            printf("Could not initialize the writers semaphore\n");
            return -1;
        }
    }

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++)
    {
        if (pthread_create(&readerThreads[i], NULL, reader, NULL))
        {
            printf("Could not create reader thread %d\n", i);
            return -1;
        }
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++)
    {
        if (pthread_create(&writerThreads[i], NULL, writer, NULL))
        {
            printf("Could not create writer thread %d\n", i);
            return -1;
        }
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

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (sem_destroy(&readers[i]) != 0)
        {
            printf("Failed to destroy the readers semaphore at index %d\n", i);
        }
        if (sem_destroy(&writers[i]) != 0)
        {
            printf("Failed to destroy the writers semaphore at index %d\n", i);
        }
    }

    return 0;
}
