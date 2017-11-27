/* A Queue (FIFO)  implementation using doubly-linked list. All operations are based on page 70, section 2.4 "Data Structures and 
 *   Algorithms by Aho, Hopcraft and Ullman".
 *
 * VERSION 0.0
 *
 */
 
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
///위에 내가 추가
enum { VAL_SUCC = 0, VAL_ERR = 1};
 
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
 
struct dlqlist
{
  struct _Thread* head;
  struct _Thread* tail;
};
 /* head and tail pointers for ready queue */ 
Thread* 	ReadyQHead;
Thread*		ReadyQTail;

/* head and tail pointers for waiting queue */
Thread*		WaitQHead;
Thread*		WaitQTai;
 
int Ready_enqueue(int i);
int Ready_dequeue();
void Ready_print_queue();
int enqueue(struct dlqlist*, int);
int dequeue(struct dlqlist*);
struct _Thread* front(struct dlqlist*);
void makenull(struct dlqlist*);
int empty(struct dlqlist*);
void print_queue(struct dlqlist* );
 
 
int dequeue_anywhere(struct dlqlist*s , int tidd);
void remove_element(struct dlqlist* s, struct _Thread* d);
int peek(struct dlqlist* s);
 
//추가
void printids(const char *s)
{
    pid_t pid;
    pthread_t tid;
    pid=getpid();
    tid=pthread_self();
    //printf("%s pid %u tid %u (0x%x) \n",s, (unsigned int)pid,(unsigned int ) tid, (unsigned int)tid);
	//InsertAtTail(tid);
	Ready_enqueue(tid);
}
void *thr_fn(void *arg)
{
    printids(" New thread : ");
    return((void *)0);
}


int main(void)
{
  // struct dlqlist* s = malloc(1 * sizeof *s);
  // if(NULL == s)
  //   {
  //     fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
  //     return EXIT_FAILURE;
  //   }
  // else
  //   {
  //     s->head = s->tail = NULL;
  //   }
  ReadyQHead =NULL;
  ReadyQTail=NULL;
  pthread_t ntid;
  int err;
  err=pthread_create(&ntid,NULL,thr_fn,NULL);//제어의 새로운 흐름을 만듦
	sleep(1);
    // Ready_enqueue(10);
    // Ready_enqueue(9);
    // Ready_enqueue(8);
    Ready_print_queue();
    err=pthread_create(&ntid,NULL,thr_fn,NULL);//제어의 새로운 흐름을 만듦
    sleep(1);
    Ready_print_queue();
    printids("Main _Thread : ");
    sleep(1);
    Ready_print_queue();
    Ready_dequeue();
    Ready_print_queue();
    
    exit(0);
    
    printf("\n\n----------------------------\n");
    
  //print_queue(s);
  //printf("dequeue test : %d \n",peek(s));//출력값은 성공값만 나오나보네
  // Ready_dequeue();
  // Ready_print_queue();
  //print_queue(s);
  //dequeue_anywhere(s,12);
  //print_queue(s);
 
  /*
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
 
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
 
 
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
  */
  return EXIT_SUCCESS;
}
//임의로 추가
int Ready_enqueue(int i)
{
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = VAL_ERR;
  //   }
  if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->tid = i;
      p->pPrev = p->pNext = NULL;
 
      ReadyQHead = ReadyQTail = p;
      ret = VAL_SUCC;
    }
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->tid = i;
      p->pPrev = p->pNext = NULL;
 
      ReadyQTail->pNext = p;
      p->pPrev = ReadyQTail;
      ReadyQTail = p;
      ret = VAL_SUCC;
    }
    }
 
  return ret;
}

int Ready_dequeue(struct dlqlist* s)
{
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = VAL_ERR;
  //   }
  if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      printf("Nothing to Dequeue()\n");
      ret = VAL_SUCC;
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct _Thread* p = ReadyQHead;
      if(NULL == ReadyQHead->pNext && NULL == ReadyQTail->pNext) /* if last element */
    {
      ReadyQHead = ReadyQTail = NULL;
    }
      else
    {
      ReadyQHead = ReadyQHead->pNext;
       //ReadyQHead->pNext->pPrev=NULL;//형린이랑 추가함
    }

      free(p);
      ret = VAL_SUCC;
    }
 
  return ret;
}
void Ready_print_queue()
{
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //    }
  if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      printf("Nothing to print\n");
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = ReadyQHead;
      printf("ReadyQoutput: ");      
      while(p)
    {
      printf("%u ", (unsigned int)p->tid);
      p = p->pNext;
    }
    printf("\n");
    
    }
}
 
/* Adds an element to tail of Queue */
int enqueue(struct dlqlist* s, int i)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->tid = i;
      p->pPrev = p->pNext = NULL;
 
      s->head = s->tail = p;
      ret = VAL_SUCC;
    }
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->tid = i;
      p->pPrev = p->pNext = NULL;
 
      s->tail->pNext = p;
      p->pPrev = s->tail;
      s->tail = p;
      ret = VAL_SUCC;
    }
    }
 
  return ret;
}
 
 
 
int dequeue(struct dlqlist* s)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to Dequeue()\n");
      ret = VAL_SUCC;
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct _Thread* p = s->head;
      if(NULL == s->head->pNext && NULL == s->tail->pNext) /* if last element */
    {
      s->head = s->tail = NULL;
    }
      else
    {
      s->head = s->head->pNext;
    }
 
      free(p);
      ret = VAL_SUCC;
    }
 
  return ret;
}
 
 
int dequeue_anywhere(struct dlqlist*s , int tidd)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to Dequeue()\n");
      ret = VAL_SUCC;
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct _Thread* p = s->head;
      for(; p; p = p->pNext)
    {
      if(tidd == p->tid)
        {
          remove_element(s,p);
        }
    }
      ret = VAL_SUCC;
    }
 
  return ret;
}
 
 
void remove_element(struct dlqlist* s, struct _Thread* d)
{
  if(NULL == d->pNext && (NULL == s->head->pNext && NULL == s->tail->pNext)) /* only one element in queue */
    {
      s->head = s->tail = NULL;
    }
  else if((NULL == d->pNext) && d->pPrev) /* removing tail */
    {
      s->tail = d->pPrev;
      d->pPrev->pNext = NULL;
    }
  else if(d->pNext && (NULL == d->pPrev)) /* removing head */
    {
      s->head = d->pNext;
      s->head->pPrev = NULL;
    }
  else /* removing from center or somewhere */
    {
      d->pPrev->pNext = d->pNext;
      d->pNext->pPrev = d->pPrev;
    }
 
  free(d);
}
 
void remove_element_2(struct dlqlist* s, struct _Thread* d)
{
  if(NULL == d->pNext)
    {
      s->tail = d->pPrev;
    }
  else
    {
      d->pNext->pPrev = d->pPrev;
    }
 
  if(NULL == d->pPrev)
    {
      s->head = d->pNext;
    }
  else
    {
      d->pPrev->pNext = d->pNext;
    }
 
  free(d);
 
 
}
 
 
void print_queue(struct dlqlist* s)
{
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
     }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to print\n");
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = s->head;
      while(p)
    {
      printf("tid = %u\n", (unsigned int)p->tid);
      p = p->pNext;
    }
    }
}
int peek(struct dlqlist* s)
{
        struct _Thread* p = s->head;
        return p->tid;
}