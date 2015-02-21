#include <stdio.h>
#include <math.h>
#include <stdbool.h>
//#include <EnumToString.h>
//#include <pthread.h>
//#include <sched.h>
//pthread_mutex_t qeueMutex;

typedef enum{GS, RS, EE} studentType;
#define TotalStudents 75
#define totalTime 120
#define max_in_Section 20

int first_Section[max_in_Section];
int second_Section[max_in_Section];
int third_Section[max_in_Section];
bool sucessAdd;
int GS_front = -1, RS_front = -1, EE_front = -1;
int GS_rear = -1, RS_rear = -1, EE_rear = -1;

 struct Student_struct{
	int id;
	studentType type; 
	int time_in_Queue;

 };	typedef struct Student_struct Student;

struct info* Students;

Student GS_students[TotalStudents];
Student RS_students[TotalStudents];
Student EE_students[TotalStudents];

int main()
{
	printf("\t size: %d \n", sizeof(GS_students)/12);

	printf("capacity: %d \n", sizeof(first_Section)/4);


	Student bob;
	bob.id = 100;
	bob.type = GS; //  (rand() % EE);
	add_to_Queue(bob, GS_students, &GS_front, &GS_rear);

	if ((sizeof(first_Section) / 4) <= max_in_Section)
	{
		Student candidate = remove_from_Queue
		 add_to_section(, first_Section);
		 sucessAdd = true;
	}
	else
	{
		sucessAdd = false;
		
	}
		return 0;
}

int add_to_section(Student s, Student *queue, int *section ){
	

	return -1;
}


void add_to_Queue(Student s, Student *queue, int *front, int *rear)
{
	if (*rear == sizeof(GS_students)/12 - 1){
		printf("\n Queue is Full!");
		return;
	}
	if (*front == -1){
		*front = 0;
	}

	*rear = *rear + 1;
	queue[*rear] = s;
}

Student remove_from_Queue(Student s, Student *queue, int *front, int *rear)
{
	Student removed;

	if (*front == -1 || *front == *rear + 1){
		printf("Queue is empty \n");
		return;
	}
	removed = queue[*front];

	return removed;
}

void display(){

	
}
