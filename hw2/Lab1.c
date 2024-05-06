#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS 4

char *name[] = {"Thitichaya", "Pounglaowech"};
char *institute[] = {"Thammasat school of Engineering"};

int	ft_strlen(const char *s)
{
	int	i;

	if (*s == '\0' || s == NULL)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

void *printData(void *threadid)
{
    int *id_ptr;
    int taskid;

    id_ptr = (int *)threadid;
    taskid = *id_ptr;
    // printf("[%d]\n", taskid);
    if (taskid == 2)
        printf("Thread %d: %s\n", taskid+1, institute[0]);
    else if (taskid == 3)
        printf("Thread %d: There are %d charecters in institute name\n", taskid+1, ft_strlen(institute[0]));
    else
        printf("Thread %d: %s\n", taskid+1, name[taskid]);
    pthread_exit(NULL);
}

int main(int ac, char **av)
{
    pthread_t threads[NUM_THREADS];
    int *taskids[NUM_THREADS];
    int rc, t;

    for (t = 0; t < NUM_THREADS; t++)
    {
        taskids[t] = (int *)malloc(sizeof(int));
        *taskids[t] = t;
        printf("Creating thread %d\n", t+1);
        rc = pthread_create(&threads[t], NULL, &printData, (void *)taskids[t]);
        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    pthread_exit(NULL);
}