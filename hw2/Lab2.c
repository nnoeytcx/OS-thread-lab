#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 64
#define MAX_VALUE 2147483647

int order;

void *countNumber(void *threadid)
{
    int *id_ptr;
    int taskid;
    int i;

    i = 0;
    id_ptr = (int *)threadid;
    taskid = *id_ptr;
    printf("Thread %d: is Counting...\n", taskid+1);
    // count here
    while (i < MAX_VALUE)
        i++;
    printf("Thread %d: is done counting in <%d> place!\n", taskid+1, order);
    order++;
    pthread_exit(NULL);
}

int main(int ac, char **av)
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;
    order = 1;

    for (t = 0; t < NUM_THREADS; t++)
    {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        printf("Creating thread %d\n", t+1);
        rc = pthread_create(&threads[t], NULL, &countNumber, (void *)taskids[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}