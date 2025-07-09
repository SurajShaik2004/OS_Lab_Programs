/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

// producer and consumer problem

#define Threads 2
#define Size 10

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

sem_t Empty, Full, mutex;
int Buffer[Size];
int in = 0, out = 0;

void *producer(void *args)
{
    while (1)
    {
        int val = rand() % 100;
        sem_wait(&Empty);
        sem_wait(&mutex);
        Buffer[in] = val;
        in = (in + 1) % Size;
        printf("placing :%d\n", val);
        sem_post(&Full);
        sem_post(&mutex);
        sleep(0.5);
    }
}

void *consumer(void *args)
{
    while (1)
    {
        int consume;
        sem_wait(&Full);
        sem_wait(&mutex);
        consume = Buffer[out];
        out = (out + 1) % Size;
        printf("consumed : %d\n", consume);
        sem_post(&Empty);
        sem_post(&mutex);
        sleep(1);
    }
}

void main()
{
    sem_init(&Empty, 0, 10);
    sem_init(&Full, 0, 2);
    // if we initialize full to 0 it acts as binary semaphore but i need counting semaphore with initial value as 0
    // thats why i did like this.
    sem_wait(&Full);
    sem_wait(&Full);
    // Binary semaphore with initial value as 1.
    sem_init(&mutex, 0, 1);

    // to generate random numbers using stdlib header file.
    srand(time(NULL));

    // creating Threads

    pthread_t Thread[Threads];

    for (int i = 0; i < Threads; i++)
    {
        if (i % 2 == 0)
        {
            if (pthread_create(&Thread[i], NULL, &producer, NULL) != 0)
            {
                printf("thread is not created.");
                return;
            }
        }
        else
        {
            if (pthread_create(&Thread[i], NULL, &consumer, NULL) != 0)
            {
                printf("thread is not created.");
                return;
            }
        }
    }

    printf("Threads are Created.");

    for (int i = 0; i < Threads; i++)
    {
        if (pthread_join(Thread[i], NULL) != 0)
        {
            printf("Thread join fail");
        }
    }

    sem_destroy(&Empty);
    sem_destroy(&Full);
    sem_destroy(&mutex);
}