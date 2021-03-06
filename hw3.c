#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>

pthread_mutex_t queueMutex;
pthread_mutex_t secOneMutex;
pthread_mutex_t secTwoMutex;
pthread_mutex_t secThreeMutex;
pthread_mutex_t printMutex;
sem_t removeAlarm;

typedef enum{GS, RS, EE} studentType;
#define TotalStudents 75
#define totalTime 120
#define max_in_Section 20
#define COUNT 1

int ID_BASE = 100;
int firstPrint = 1;
int firstSecCounter = 0;
int GS_front = -1, RS_front = -1, EE_front = -1;
int GS_rear = -1, RS_rear = -1, EE_rear = -1;
time_t startTime;

 struct Student_struct{
	int id;
	studentType type;
	int time_in_Queue;
	int section_pref;
	int max_wait_time;
	int arrival_time;
	int turn_around_time;

 };	typedef struct Student_struct Student;

struct info* Students;
Student first_Section[max_in_Section];
int second_Section[max_in_Section];
int third_Section[max_in_Section];
Student GS_students[TotalStudents];
Student RS_students[TotalStudents];
Student EE_students[TotalStudents];

// Function Declarations
Student remove_from_Queue(Student *queue, int *front, int *rear);
void add_to_Queue(Student s, Student *queue, int *front, int *rear);
void *student(void *param);
void enroll(Student *potentialStudent);
void *stopwatch(void *param);
void print(char *event);

int main()
{
	// Seed the random number generator with the time
	srand(time(0));
	time(&startTime);

	int id = 0;
	pthread_t stopWatchId;
	pthread_attr_t stopWatchAttr;
	pthread_attr_init(&stopWatchAttr);
	pthread_create(&stopWatchId, &stopWatchAttr, stopwatch, &id);

	int threadId[75];
	int i;
	for (i = 0; i<COUNT; i++)
	{
		threadId[i] = ID_BASE + i;
		pthread_t testThreadId;
		pthread_attr_t testThreadAttr;
		pthread_attr_init(&testThreadAttr);
		pthread_create(&testThreadId, &testThreadAttr, student, &threadId[i]);
	}
	pthread_join(stopWatchId, NULL);
	
	Student s;
	printf("\n");
	printf("Section 1\n");
	printf("------------------------------------\n");
	printf("STUDENT | STATUS   | TURN AROUND TIME \n");
	for(i=0; i<=firstSecCounter; i++)
	{
		s = first_Section[i]; 
		int min=0;
		int sec = s.turn_around_time;
		if( sec >= 60)
		{
			min++;
			sec -= 60;
		}
		printf("%d     | Enrolled | %1d:%02d\n", s.id, min, sec);
	}
		
	return 0;
}

void *student(void *param)
{
	Student potentialStudent;
	potentialStudent.id = *((int *)param);
	potentialStudent.section_pref = rand() % 3;
	potentialStudent.max_wait_time = 10;
	potentialStudent.type = GS;

	// Sleep the process from 0 - 120 seconds
	int wakeUpTime = rand() % 5; //totalTime;
	char event[80];
	sprintf(event, "Created student %d, now sleep for %d", potentialStudent.id, wakeUpTime);
	print(event);
	sleep(wakeUpTime);

	// Get the current time and add it to the arrival time
	time_t now;
	time(&now);
	potentialStudent.arrival_time = now;
	
	// Add the student to the queue
	enroll(&potentialStudent);
	return NULL;
}

void enroll(Student *potentialStudent)
{
	char event[80];
	if(potentialStudent->type == GS)
	{
		sprintf(event, "Student %d arrived joined the GS queue", potentialStudent->id);
		print(event);
		add_to_Queue(*potentialStudent, GS_students, &GS_front, &GS_rear);

		if(GS_rear == GS_front)
		{
			// Get the next student
			remove_from_Queue(GS_students, &GS_front, &GS_rear);
			// Sleep from 1 to 2 seconds
			sleep((rand()%2) + 1);
			sprintf(event, "Student %d was removed from the GS queue", potentialStudent->id);
			print(event);
		}
	}

	pthread_mutex_lock(&secOneMutex);
	if(potentialStudent->section_pref == 1)
	{
		if (firstSecCounter <= max_in_Section)
		{
			time_t now;
			time(&now);
			double elapsed = difftime(now, potentialStudent->arrival_time);
			int turnAroundTime = (int) elapsed;

			potentialStudent->turn_around_time = turnAroundTime;
			first_Section[firstSecCounter++] = *potentialStudent;

			sprintf(event, "Student %d enrolled in section 1", potentialStudent->id);
			print(event);
		}	
	}
	pthread_mutex_unlock(&secOneMutex);
}

void *stopwatch(void *param)
{
	sleep(30);
	char event[80];
	sprintf(event, "Enrollment is over");
	print(event);
	return NULL;
}
void add_to_Queue(Student s, Student *queue, int *front, int *rear)
{
	pthread_mutex_lock(&queueMutex);

	if (*rear == sizeof(GS_students)/12 - 1){
		printf("\n Queue is Full!");
		return;
	}
	if (*front == -1){
		*front = 0;
	}

	*rear = *rear + 1;
	queue[*rear] = s;

	pthread_mutex_unlock(&queueMutex);
}

Student remove_from_Queue(Student *queue, int *front, int *rear)
{
	pthread_mutex_lock(&queueMutex);
	Student removed;

	if (*front == -1 || *front == *rear + 1){
		printf("Queue is empty \n");
		return;
	}
	removed = queue[*front];

	pthread_mutex_unlock(&queueMutex);
	return removed;
}

void print(char *event)
{
	time_t now;
	time(&now);
	double elapsed = difftime(now, startTime);
	int min = 0;
	int sec = (int) elapsed;

	if( sec >= 60)
	{
		min++;
		sec -= 60;
	}

	// Acquire the mutex lock to protect the printing
	pthread_mutex_lock(&printMutex);
	
	if(firstPrint) 
	{
		printf("TIME | EVENT\n");
		firstPrint = 0;
	}

	// Elapsed time.
	printf("%1d:%02d | ", min, sec);

	// What even occurred.
	printf("%s\n", event);

	// Release the mutex lock
	pthread_mutex_unlock(&printMutex);
}
