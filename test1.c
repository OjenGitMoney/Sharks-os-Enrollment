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
#define MaxLineLenght 75
#define ID_BASE 100
int counter = 0;

//int class[COUNT];
int Section1[Capacity];     // circular buffer of section1
int Section2[Capacity];     // circular buffer of section1
int Section3[Capacity];     // circular buffer of section1
int NEnrollS1 =0;// initial empty
int NEnrollS2 =0;// initial empty
int NEnrollS3 =0;// initial empty
int LGS[MaxLineLenght];     // circular buffer of line GS
int LRS[MaxLineLenght];     // circular buffer of line RS
int LEE[MaxLineLenght];     // circular buffer of line EE

int HGS =0;// initial empty
int TGS =0;// initial empty
int HRS =0;// initial empty
int TRS =0;// initial empty
int HEE =0;// initial empty
int TEE =0;// initial empty

int startTime=0;
bool SignalSuicide =false;
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
// Elapsed time.
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


void enroll(struct student s)
{
	if(s.section_pref==1 || s.section_pref==0)
	{
// enroll in section 1
		if(NEnrollS1<Capacity )
		{
				 pthread_mutex_lock(&S1Mutex);
				 Section1[NEnrollS1]=s.id;
			
				 s.positioninS = NEnrollS1;
				 printf("%d :\n", s.positioninS);
				 NEnrollS1++;
				 pthread_mutex_unlock(&S1Mutex);
				 if(NEnrollS1==Capacity)
				 {
					 Terminate();
				 }
		}
	}
	if(s.section_pref==2)
	{
// enroll in section 2
				if(NEnrollS2<Capacity || s.section_pref==0 )
		{
				 pthread_mutex_lock(&S2Mutex);
				 Section2[NEnrollS2]=s.id;
				 s.positioninS = NEnrollS2;
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
				 Section3[NEnrollS3]=s.id;
				 s.positioninS = NEnrollS3;
				 NEnrollS3++;
				 pthread_mutex_unlock(&S3Mutex);
				 if(NEnrollS3==Capacity)
				 {
					 Terminate();
				 }
		}
	}



}


void SimLineGS(struct student s)
{
	
				 pthread_mutex_lock(&GSMutex);
				  LGS[TGS]=s.id;
				  s.positioninL = TGS;
				  TGS++;
				  PrintL(1,s);
				  pthread_mutex_unlock(&GSMutex);
			  


			 while (s.positioninL!=HGS)
			 {
				// waiting until turn
			 }
				//  printf("IN THE ENROLL %d\n",s.id);
				  // enroll
				  sleep(1+rand() % 1);
				 // sleep(rand() % 2);
				   pthread_mutex_lock(&GSMutex);
						HGS++;			
				  pthread_mutex_unlock(&GSMutex);
				    enroll(s);
					PrintL(0,s); 
}




void SimLineRS(struct student s)
{
	
				 pthread_mutex_lock(&RSMutex);
				  LRS[TRS]=s.id;
				  s.positioninL = TRS;
				  TRS++;
				  PrintL(1,s);
				  pthread_mutex_unlock(&RSMutex);

			 while (s.positioninL!=HRS)
			 {
				 // waiting for signal to move up the line				
			 }
 
				  // enroll
				  sleep(2+rand() % 2);
			
				   pthread_mutex_lock(&RSMutex);
						 HRS++;
				  pthread_mutex_unlock(&RSMutex);
				  	  enroll(s);
					  PrintL(0,s);
	 
}





void SimLineEE(struct student s)
{
	
				 pthread_mutex_lock(&EEMutex);
				  LEE[TEE]=s.id;
				  s.positioninL = TEE;
				  TEE++;
				  PrintL(1,s);
				  pthread_mutex_unlock(&EEMutex);
			  
			 while (s.positioninL!=HEE)
			 {
				 // waiting for signal to move up the line			 
			 }
				  // enroll
				  sleep(3+rand() % 3);
				
				   pthread_mutex_lock(&EEMutex);
						 HEE++;
				  pthread_mutex_unlock(&EEMutex);
				   enroll(s);
				  PrintL(0,s);
			  
	 
}








void flow(struct student s)
{
			    sleep(2);
			 // sleep(s.arrival_time);
			   if (s.type == GS)
		       {
				   SimLineGS(s);
			   }
			   if (s.type == RS)
		       {
				   SimLineRS(s);
			   }
			   if (s.type == EE)
		       {
				   SimLineEE(s);
			   }
	
			//	pthread_mutex_lock(&testCMutex);
			//	testC = s.id;

				//printf("Process");
			//	int a;
			////	for (a=0; a<100;a<a++)
				//{
					//printf("Process done %d is sleeping for %d Type : %d \n", s.id,s.arrival_time, s.type);
			//	}	
			//	pthread_mutex_unlock(&testCMutex);
}

void *student(void *param)
{
  // 	pthread_mutex_lock(&estCMutex);

	struct student s;
	s.id = *((int *)param);
	//init turn around time
	s.section_pref = rand() % 3;
	s.patient_time = 10;
	s.arrival_time = rand() % 120;
	s.type = (EnumType) (rand() % 3);
	// this is for testing each case
	//s.type = (EnumType) (2);

//	printf("Process %d is sleeping for %d \n", s.id, wakeUpTime);
	printf("Process %d is sleeping for %d Type : %d \n", s.id, s.arrival_time, s.type);
	
	flow(s);


//	time_t now;
//	s.arrival_time = now;
	
	//pthread_mutex_unlock(&estCMutex);
//	enroll(&s);
	return NULL;
}



int main()
{
	time_t startTimeo;
	time(&startTimeo);
	startTime=(int)startTimeo;
    int id = 0;
    int rc;
	pthread_t testThreadId[NUM_THREADS];
    pthread_attr_t testThreadAttr;
    int threadId[NUM_THREADS];
    int i;
    for(i=0; i<NUM_THREADS; i++)
    {
        threadId[i] = ID_BASE + i;
		threadId[i] = i;
        pthread_attr_init(&testThreadAttr);
        rc = pthread_create(&testThreadId[i], &testThreadAttr, student, &threadId[i]);
	    if (rc)
	       {
               printf("ERROR; return code from pthread_create() is %d\n", rc);    
           }
    }

	// wait for the thread to end
	int ii;
   for (ii =0; ii < NUM_THREADS; ii++)
   {
	   pthread_join(testThreadId[ii], NULL);
   }
  // pthread_exit(NULL);
    return 0;
}
