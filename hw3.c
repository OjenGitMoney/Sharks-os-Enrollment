#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include <EnumToString.h>
//#include <pthread.h>
//#include <sched.h>
pthread_mutex_t qeueMutex;
pthread_mutex_t secOneMutex;
pthread_mutex_t secTwoMutex;
pthread_mutex_t secThreeMutex;
semt_t removeAlarm;

typedef enum{GS, RS, EE} studentType;
#define TotalStudents 75
#define totalTime 120
#define max_in_Section 20
#define COUNT 1

int ID_BASE = 100;
int time;
int first_Section[max_in_Section];
int second_Section[max_in_Section];
int third_Section[max_in_Section];
bool sucessAdd;
int GS_front = -1, RS_front = -1, EE_front = -1;
int GS_rear = -1, RS_rear = -1, EE_rear = -1;


 struct Student_struct{
	int id;
	studentType type;
	int section_preffered;
	int time_in_Queue;
	int section_pref;
	int arrival_time;

 };	typedef struct Student_struct Student;

struct info* Students;

Student GS_students[TotalStudents];
Student RS_students[TotalStudents];
Student EE_students[TotalStudents];



void flow(Student *s)
{
	if (s->type == GS){
	add_to_Queue( *s, GS_students, &GS_front, &GS_rear);
	if (GS_rear == GS_front){
		remove_from_Queue(*s, GS_students, &GS_front, &GS_rear);
		sem_post();
	}
	if (GS_rear > GS_front ){
		while ()
	}

	//remove_from_Queue()

	}
	if (s->type == RS){
		add_to_Queue(*s, RS_students, RS_front, RS_rear);
	}
	if (s->type == EE){
		add_to_Queue(*s, EE_students, EE_front, EE_rear);
	}
	
	if ( GS_students->)


	//rem to put Lock Mutex
	if ( < max_in_Section)
	{
		first_Section[] = s->id;
		printf("Successfully added student %d\n", s->id);
		counter++;
	}
	else
	{
		printf("Class is full, could not enroll student %d\n", s->id);
	}
	//Unlock Mutex
}

void *student(void *param)
{
	Student s;
	s.id = *((int *)param);
	s.section_pref = rand() % 3;
	int wakeUpTime = rand() % totalTime;
	printf("Process %d is sleeping for %d\n", s.id, wakeUpTime);
	sleep(wakeUpTime);
	time_t now;
	s.arrival_time = now;

	enroll(&s);
	return NULL;
}

int main()
{
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

//	add_to_Queue(bob, GS_students, &GS_front, &GS_rear);

	if ((sizeof(first_Section) / 4) <= max_in_Section)
	{
		//Student candidate = remove_from_Queue
		// add_to_section(, first_Section);
		 sucessAdd = true;
	}
	else
	{
		sucessAdd = false;
		
	}
		return 0;
}


void *stopwatch(void *param)
{
	sleep(120);
	return NULL;
}



void add_to_Queue(Student s, Student *queue, int *front, int *rear)
{
	pthread_mutex_lock(&qeueMutex);

	if (*rear == sizeof(GS_students)/12 - 1){
		printf("\n Queue is Full!");
		return;
	}
	if (*front == -1){
		*front = 0;
	}

	*rear = *rear + 1;
	queue[*rear] = s;

	pthread_mutex_unlock(&qeueMutex);
}

Student remove_from_Queue(Student s, Student *queue, int *front, int *rear)
{
	pthread_mutex_lock(&qeueMutex);
	Student removed;

	if (*front == -1 || *front == *rear + 1){
		printf("Queue is empty \n");
		return;
	}
	removed = queue[*front];

	return removed;
	pthread_mutex_unlock(&qeueMutex);
}

void display(){

	
}
