/* Doubly Linked List implementation */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/types.h>
typedef struct _Thread Thread;
typedef struct _Thread  {//_Thread __Thread 이거 일단 반대로해봄
	pthread_t data;
	struct _Thread* next;
	struct _Thread* prev;
}Thread;

struct _Thread* head; // global variable - pointer to head _Thread.

//Creates a new _Thread and returns pointer to it. 
struct _Thread* GetNew_Thread(pthread_t x) {
	struct _Thread* new_Thread
		= (struct _Thread*)malloc(sizeof(struct _Thread));
	new_Thread->data = x;
	new_Thread->prev = NULL;
	new_Thread->next = NULL;
	return new_Thread;
}

//Inserts a _Thread at head of doubly linked list
void InsertAtHead(pthread_t x) {
	struct _Thread* new_Thread = GetNew_Thread(x);
	if(head == NULL) {
		head = new_Thread;
		return;
	}
	head->prev = new_Thread;
	new_Thread->next = head; 
	head = new_Thread;
}

//Inserts a _Thread at tail of Doubly linked list
void InsertAtTail(int x) {
	struct _Thread* temp = head;
	struct _Thread* new_Thread = GetNew_Thread(x);
	if(head == NULL) {
		head = new_Thread;
		return;
	}
	while(temp->next != NULL) temp = temp->next; // Go To last _Thread
	temp->next = new_Thread;
	new_Thread->prev = temp;
}

//Prints all the elements in linked list in forward traversal order
void Print() {
	struct _Thread* temp = head;
	printf("Forward: ");
	while(temp != NULL) {
		printf("%u ",(unsigned int)temp->data);
		temp = temp->next;
	}
	printf("\n");
}

//Prints all elements in linked list in reverse traversal order. 
void ReversePrint() {
	struct _Thread* temp = head;
	if(temp == NULL) return; // empty list, exit
	// Going to last _Thread
	while(temp->next != NULL) {
		temp = temp->next;
	}
	// Traversing backward using prev pointer
	printf("Reverse: ");
	while(temp != NULL) {
		printf("%u ",(unsigned int)temp->data);
		temp = temp->prev;
	}
	printf("\n");
}
void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid=getpid();
    tid=pthread_self();
    //printf("%s pid %u tid %u (0x%x) \n",s, (unsigned int)pid,(unsigned int ) tid, (unsigned int)tid);
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