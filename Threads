#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>

#define COUNT 20
#define ID_BASE 100
int counter = 0;
int class[COUNT];

typedef enum {GS, RS, EE} classlevel;

struct student {
    int id;
    int section;
    classlevel type;
};


void enroll(struct student *s)
{
    if(counter < 10)
    {
        class[counter] = s->id;
        printf("Successfully added student %d\n", s->id);
        counter++;
    }
    else
    {
        printf("Class is full, could not enroll student %d\n", s->id);
    }
}

void *student(void *param)
{
    struct student s;
    s.id = *((int *) param);
    s.section = 1;
    s.section = 1;
    s.type = GS;
    int time = rand()%60;
    printf("Process %d is sleeping for %d\n", s.id, time);
    sleep(time);
    enroll(&s);
    return NULL;
}

void *stopwatch(void *param)
{
    sleep(100);
    return NULL;
}

int main()
{
    int id = 0;
    pthread_t stopWatchId;
    pthread_attr_t stopWatchAttr;
    pthread_attr_init(&stopWatchAttr);
    pthread_create(&stopWatchId, &stopWatchAttr, stopwatch, &id);

    int threadId[20];
    int i;
    for(i=0; i<COUNT; i++)
    {
        threadId[i] = ID_BASE + i;
        pthread_t testThreadId;
        pthread_attr_t testThreadAttr;
        pthread_attr_init(&testThreadAttr);
        pthread_create(&testThreadId, &testThreadAttr, student, &threadId[i]);
    }
    pthread_join(stopWatchId, NULL);
    return 0;
}
