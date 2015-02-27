#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#define NUM_THREADS     15
#define Capacity 20
#define MAXLINELENGTH 75
#define ID_BASE 100
#define STOPTIME = 120


int firstPrint = 1;
typedef enum {GS, RS, EE} EnumType;

struct student {
    int id;
//	studentType type;
	int time_in_Queue;
	int positioninL;
	int positioninS;
	int turnaroundTime;
	int section_pref;
	int patient_time;
	int arrival_time;
    EnumType type;
};
//int class[COUNT];
student Section1[Capacity];     // circular buffer of section1
student Section2[Capacity];     // circular buffer of section1
student Section3[Capacity];     // circular buffer of section1
student notEnrolled[MAXLINELENGTH];
int dropCounter = 0; // Keep track of dropped students
int NEnrollS1 =0;// initial empty
int NEnrollS2 =0;// initial empty
int NEnrollS3 =0;// initial empty
int LGS[MAXLINELENGTH];     // circular buffer of line GS
int LRS[MAXLINELENGTH];     // circular buffer of line RS
int LEE[MAXLINELENGTH];     // circular buffer of line EE

int HGS =0;// initial empty
int TGS =0;// initial empty
int HRS =0;// initial empty
int TRS =0;// initial empty
int HEE =0;// initial empty
int TEE =0;// initial empty

int startTime=0;
bool SignalSuicide =false;

sem_t gs_sem_go;
sem_t rs_sem_go;
sem_t ee_sem_go;

pthread_mutex_t printMutex;  // mutex protects printing
pthread_mutex_t GSMutex;
pthread_mutex_t RSMutex;
pthread_mutex_t EEMutex;
pthread_mutex_t S1Mutex;
pthread_mutex_t S2Mutex;
pthread_mutex_t S3Mutex;
pthread_mutex_t KKMutex;
pthread_mutex_t TMutex;
//pthread_mutex_t testCMutex;  // mutex protects chairs and wait count
//pthread_mutex_t estCMutex;  // mutex protects chairs and wait count
//int testC =0;
     
void PrintL(int IO,struct student s)
{
    // Acquire the mutex lock to protect the printing.
  //  time(&startTime);
	time_t now;
    time(&now);
    double elapsed = difftime(now, startTime);
    int min = 0;
    int sec = (int) elapsed;
	//printf("%d\n",sec);
    if (sec >= 60) {
        min++;
        sec -= 60;
    }
	 pthread_mutex_lock(&printMutex);
    if (firstPrint) {
        printf("TIME |  Section-Tudent |    WAITING|       EVENT\n");
        firstPrint = 0;
    }
	printf("--------------%d:%d -------------------------\n",min, sec);
   
// Who's in the line GS.
    if (HGS < TGS) {
		  printf("Line GS  is: ");
		int b1;
		for(b1=HGS; b1 < TGS; b1++)
		{
			printf("%2d,",LGS[b1] );
		}
         printf("        \n");
    }

// End Who's in the line GS.
	
// Who's in the line RS.
    if (HRS < TRS) {
	printf("Line RS  is: ");
		int b2;
		for(b2=HRS; b2 < TRS; b2++)
		{
			printf("%2d,",LRS[b2] );
		}
         printf("        \n");
    }
 //   else {
 //       printf("        \n");
 //   }
//End Who's in the line RS.

// Who's in the line EE.
    if (HEE <TEE) {
		printf("Line EE  is: ");
		int b3;
		for(b3=HEE; b3 < TEE; b3++)
		{
			printf("%2d,",LEE[b3] );
		}
         printf("        \n");
    }

//end  Who's in the line EE.

	  // Who's in the section1.
    if (NEnrollS1 > 0) {
		printf("Section 1 is: ");
		int a1;
		for(a1=0; a1 < NEnrollS1; a1++)
		{
			printf("%2d,",Section1[a1] );
		}
        printf("\n");
    }


//end  Who's in the section1.

// Who's in the section2.
    if (NEnrollS2 > 0) {
		printf("Section 2 is: ");
		int a2;
		for(a2=0; a2 < NEnrollS2; a2++)
		{
			printf("%2d,",Section2[a2] );
		}
        printf("        \n");
    }

//end  Who's in the section2.

// Who's in the section3.
    if (NEnrollS3 > 0) {
		printf("Section 3 is: ");
		int a3;
		for(a3=0; a3 < NEnrollS3; a3++)
		{
			printf("%2d,",Section3[a3] );
		}
        printf("        \n");
    }

//end  Who's in the section3.
	// print event
	if(IO ==1)// input, add to the line
	{
			printf("Process %d is Arrival %d in line : %d at position :%d\n", s.id,s.arrival_time, s.type+1,s.positioninL);
	}
		if(IO ==0)// output enroll
	{
			printf("Process %d is Go in to Section %d at position : %d \n", s.id,s.section_pref, s.positioninS);
	}
	printf("-------------------------------------------------------\n");
	 pthread_mutex_unlock(&printMutex);

}

// When the alarm is triggered this function will execute

void *student(void *param)
{
	struct student s;
	s.id = *((int *)param);
	s.section_pref = rand() % 3;
	s.patient_time = 10;
	s.arrival_time = rand() % 120;
	s.type = (EnumType) (rand() % 3);
	printf("Process %d is sleeping for %d Type : %d \n", s.id, s.arrival_time, s.type);
	flow(s);
	return NULL;
}

void flow(struct student s)
{
	sleep(2);
	if (s.type == GS)
	{
		sem_wait(&gs_sem_go);
		LGS[TGS]=s;
		s.positioninL = TGS;
		TGS++;
		PrintL(1,s);
		sem_post(&gs_sem_go);
	}
	if (s.type == RS)
	{
		sem_wait(&rs_sem_go);
		LRS[TRS]=s;
		s.positioninL = TRS;
		TRS++;
		PrintL(1,s);
		sem_post(&rs_sem_go);
	}
	if (s.type == EE)
	{
		sem_wait(&ee_sem_go);
		LEE[TEE]=s;
		s.positioninL = TEE;
		TEE++;
		PrintL(1,s);
		sem_post(&ee_sem_go);
	}
}

void SimLineGS()
{
	// enroll
  	sem_wait(&gs_sem_go);	
	static int check = 0;
	if(TGS > check)
	{
		check = TGS;
		sleep(1+rand() % 1);
		struct student s = LGS[HGS];
		if(turnAroundTime(s) > s.patient_time)
			drop(s);
		else
			enroll(s);
		HGS++;
	}
	sem_post(&gs_sem_go);
	PrintL(0,s); 
}

void *gsQueueThread(void *param)
{
	do
	{
	  SimLineGS();
	} while(!timesUp);
}

void SimLineRS()
{
	// enroll
	sem_wait(&rs_sem_go);
	static int check = 0;
	if(TRS > check)
	{
		check = TRS;
		sleep(2+rand() % 2);
		struct student s = LRS[HRS];
		if(turnAroundTime(s) > s.patient_time)
			drop(s);
		else
			enroll(s);
		HRS++;
	}
	sem_post(&rs_sem_go);
	PrintL(0,s);
}

void *rsQueueThread(void *param)
{
	do
	{
	  SimLineRS();
	} while(!timesUp);
}

void SimLineEE()
{
	// enroll
	sem_wait(&ee_sem_go);
	static int check = 0;
	if (TEE > check) 
	{
		check = TEE;
		sleep(3+rand() % 3);
		enroll(LEE[HEE++]);
		struct student s = LRS[HRS];
		if(turnAroundTime(s) > s.patient_time)
			drop(s);
		else
			enroll(s);
		HEE++;
	}
	sem_post(&ee_sem_go);
	PrintL(0,s);
}

void *eeQueueThread(void *param)
{
	do
	{
	  SimLineEE();
	} while(!timesUp);
}

void terminateEnrollment(int signal)
{
	timesUp = 1;
}

void Terminate()
{
	// check other section is full
		if(NEnrollS1<Capacity)
		{
			if(NEnrollS2<Capacity)
				{
					if(NEnrollS3<Capacity)
						{
							 pthread_mutex_lock(&KKMutex);
								SignalSuicide = true;
							 pthread_mutex_unlock(&KKMutex);
						}
				}
		}
}

// Calculate the turn around time
int turnAroundTime(struct student s)
{
	time_t now;
	time(&now);
	double elapsed = difftime(now, s.arrival_time);
	int turnAroundTime = (int) elapsed;
	return turnAroundTime;
}

void drop(struct student s)
{
	notEnrolled[dropCounter++] = s;
}	

void enroll(struct student s)
{
	if(s.section_pref==1 || s.section_pref==0)
	{
		// enroll in section 1
		if(NEnrollS1<Capacity )
		{
			 pthread_mutex_lock(&S1Mutex);
			 s.positioninS = NEnrollS1;
			 s.section_pref = 1;
			 s.turn_around_time = turnAroundTime(s);
			 Section1[NEnrollS1]=s;
			 printf("%d :\n", s.positioninS);
			 NEnrollS1++;
			 pthread_mutex_unlock(&S1Mutex);
			 if(NEnrollS1==Capacity)
			 {
				 Terminate();
			 }
		}
	}
	if(s.section_pref==2 || s.section_pref==0)
	{
		// enroll in section 2
		if(NEnrollS2<Capacity)
		{
			 pthread_mutex_lock(&S2Mutex);
			 s.positioninS = NEnrollS2;
			 s.section_pref = 2;
			 s.turn_around_time = turnAroundTime(s);
			 Section2[NEnrollS2]=s;
			 NEnrollS2++;
			 pthread_mutex_unlock(&S2Mutex);
			 if(NEnrollS2==Capacity)
			 {
				 Terminate();
			 }
		}

	}
	if(s.section_pref==3 || s.section_pref==0)
	{
		// enroll in section 3
		if(NEnrollS3<Capacity)
		{
			 pthread_mutex_lock(&S3Mutex);
			 s.positioninS = NEnrollS3;
			 s.section_pref = 3;
			 s.turn_around_time = turnAroundTime(s);
			 Section3[NEnrollS3]=s;
			 NEnrollS3++;
			 pthread_mutex_unlock(&S3Mutex);
			 if(NEnrollS3==Capacity)
			 {
				 Terminate();
			 }
		}
	}
}

int main()
{
	time_t startTime; // Declare startTime
	time(&startTime); // Initialize startTime
	startTime=(int)startTime;

        struct itimerval timer; // Create timer struct
        timer.it_interval.tv_sec = 0; // Only interrupt the process once
        timer.it_interval.tv_usec = 0; // Only interrupt the process once
        timer.it_value.tv_sec = STOPTIME; // Set time to stop after 120 seconds
        timer.it_value.tv_usec = 0;
        setitimer(ITIMER_REAL, &timer, NULL); // Set timer in real time
        signal(SIGALRM, terminateEnrollment); // After 120 seconds terminate all enrollemnt queues

	sem_init(&gs_sem_go, 0, 1); // Manages the gs queue
	sem_init(&rs_sem_go, 0, 1); // Manages the rs queue
	sem_init(&ee_sem_go, 0, 1); // Manages the ee queue

        // Create gsQueue thread
        pthread_t gsQueueThreadId = 80;
	pthread_attr_t gsQueueThreadAttr;
        pthread_attr_init(&gsQueueThreadAttr);
        pthread_create(&gsQueueThreadId[i], &gsQueueThreadAttr, gsQueueThread, &gsQueueThreadId);

        // Create rsQueue thread
        pthread_t rsQueueThreadId = 81;
	pthread_attr_t rsQueueThreadAttr;
        pthread_attr_init(&rsQueueThreadAttr);
        pthread_create(&rsQueueThreadId[i], &rsQueueThreadAttr, rsQueueThread, &rsQueueThreadId);

        // Create eeQueue thread
        pthread_t eeQueueThreadId = 82;
	pthread_attr_t eeQueueThreadAttr;
        pthread_attr_init(&eeQueueThreadAttr);
        pthread_create(&eeQueueThreadId[i], &eeQueueThreadAttr, eeQueueThread, &eeQueueThreadId);

	int id = 0;
	int rc;
	int i;
	pthread_t studentThreadId[NUM_THREADS];
	int studentThreadId[NUM_THREADS];
        for(i=0; i<NUM_THREADS; i++)
        {
            studentThreadId[i] = ID_BASE + i;
            pthread_attr_t studentThreadAttr;
            pthread_attr_init(&studentThreadAttr);
            rc = pthread_create(&studentThreadId[i], &studentThreadAttr, student, &threadId[i]);
            if (rc)
            {
                printf("ERROR; return code from pthread_create() is %d\n", rc);
            }
        }

	// wait for the thread to end
        for (i=0; i < NUM_THREADS; i++)
        {
               pthread_join(testThreadId[i], NULL);
        }
        // pthread_exit(NULL);
        return 0;
}
