/* Doubly Linked List implementation */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define TIMESLICE	(2)


typedef int BOOL;
typedef pthread_t thread_t;
typedef void thread_attr_t;


typedef enum{
	THREAD_STATUS_RUN = 0,
	THREAD_STATUS_READY = 1,
	THREAD_STATUS_BLOCKED = 2,
}ThreadStatus;


typedef struct _Thread Thread;
typedef struct _Thread  {//_Thread __Thread 이거 일단 반대로해봄
	ThreadStatus status;
    pthread_t tid;
    pthread_cond_t readyCond;
   	BOOL bRunnable;
   	pthread_mutex_t	readyMutex;
	Thread*	pPrev;
	Thread*	pNext;
}Thread;
//내가 따로 추가
typedef struct __wrapperArg {
   void*  (*funcPtr)(void*);
   void* funcArg;
} WrapperArg;


/* head and tail pointers for ready queue */ 
Thread* 	ReadyQHead;
Thread*		ReadyQTail;

/* head and tail pointers for waiting queue */
Thread*		WaitQHead;
Thread*		WaitQTai;

//까지
struct _Thread* head; // global variable - pointer to head _Thread.

//Creates a new _Thread and returns pointer to it. 
struct _Thread* GetNew_Thread(pthread_t x) {
	struct _Thread* new_Thread
		= (struct _Thread*)malloc(sizeof(struct _Thread));
	new_Thread->tid = x;
	new_Thread->pPrev = NULL;
	new_Thread->pNext = NULL;
	return new_Thread;
}

//Inserts a _Thread at head of doubly linked list
// void InsertAtHead(pthread_t x) {
// 	struct _Thread* new_Thread = GetNew_Thread(x);
// 	if(head == NULL) {
// 		head = new_Thread;
// 		return;
// 	}
// 	head->pPrev = new_Thread;
// 	new_Thread->pNext = head; 
// 	head = new_Thread;
// }

//Inserts a _Thread at tail of Doubly linked list
void InsertAtTail(pthread_t x) {
	struct _Thread* temp = head;
	struct _Thread* new_Thread = GetNew_Thread(x);
	if(head == NULL) {
		head = new_Thread;
		return;
	}
	while(temp->pNext != NULL) temp = temp->pNext; // Go To last _Thread
	temp->pNext = new_Thread;
	new_Thread->pPrev = temp;
}
void queue_enqueue(pthread_t tid){
	Thread* temp;
	temp = (Thread*)malloc(sizeof(Thread));
	// if (temp == NULL)
	// 	return FAILURE;

	temp->tid = tid;
	temp->pNext = NULL;
	temp->pPrev = NULL;	
	if(temp->pPrev != NULL){
		if (ReadyQTail != NULL) {
			ReadyQTail->pNext = temp;
		}
		temp->pPrev = ReadyQTail;
		ReadyQTail = temp;
		if (ReadyQHead == NULL)
			ReadyQHead = temp;
	}
}
//Prints all the elements in linked list in forward traversal order
void Print() {
	struct _Thread* temp = head;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%u ",(unsigned int)temp->tid);
		temp = temp->pNext;
	}
	printf("\n");
}
void Ready_Print() {
	struct _Thread* temp = ReadyQHead;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%u ",(unsigned int)temp->tid);
		temp = temp->pNext;
	}
	printf("\n");
}
// //Prints all elements in linked list in reverse traversal order. 
// void ReversePrint() {
// 	struct _Thread* temp = head;
// 	if(temp == NULL) return; // empty list, exit
// 	// Going to last _Thread
// 	while(temp->pNext != NULL) {
// 		temp = temp->pNext;
// 	}
// 	// Traversing backward using pPrev pointer
// 	printf("Reverse: ");
// 	while(temp != NULL) {
// 		printf("%u ",(unsigned int)temp->tid);
// 		temp = temp->pPrev;
// 	}
// 	printf("\n");
// }
void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid=getpid();
    tid=pthread_self();
    //printf("%s pid %u tid %u (0x%x) \n",s, (unsigned int)pid,(unsigned int ) tid, (unsigned int)tid);
	//InsertAtTail(tid);
	InsertAtTail(tid);
}
void *thr_fn(void *arg)
{
    printids(" New thread : ");
    return((void *)0);
}
int main() {

	/*Driver code to test the implementation*/
	head = NULL; // empty list. set head as NULL. 
	ReadyQHead =NULL;
	ReadyQTail=NULL;
	// Calling an Insert and printing list both in forward as well as reverse direction. 
	// InsertAtTail(2); Print(); ReversePrint();
	// InsertAtTail(4); Print(); ReversePrint();
	// InsertAtHead(6); Print(); ReversePrint();
	// InsertAtTail(8); Print(); ReversePrint();
	pthread_t ntid;
	 int err;
    err=pthread_create(&ntid,NULL,thr_fn,NULL);//제어의 새로운 흐름을 만듦
	sleep(1);
		Print();
	err=pthread_create(&ntid,NULL,thr_fn,NULL);//제어의 새로운 흐름을 만듦
	sleep(1);
		Print();
    if(err!=0)
    exit(1);
    printids("Main _Thread : ");
    sleep(1);
		Print();
    exit(0);
}