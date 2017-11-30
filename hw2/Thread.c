#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <signal.h>
#define TRUE true
#define FALSE false
static pthread_cond_t bcond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t bmutex = PTHREAD_MUTEX_INITIALIZER;
void (*old_fun)( int);

void *child(void *arg) {
   printf("child\n");
   return NULL;
}

void* __wrapperFunc(void* arg)
{
Ready_enqueue(thread_self());
printf("ReadQ insert tid : %u\n",(unsigned int)thread_self());
  void* ret;
  WrapperArg* pArg = (WrapperArg*)arg;
  sigset_t set;
  int retSig;

  // child sleeps until TCB is initialized
     signal(SIGUSR1, __thread_wait_handler);//핸들러 등록
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigwait(&set, &retSig);
  // child is ready to run

  __thread_wait_handler(0);//다시 재움
  void* funcPtr = pArg->funcPtr;
  void* funcArg = pArg->funcArg;
  
  ret = (*pArg->funcPtr)(pArg->funcArg);
  
  return ret;

}

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    //Thread temp;
    //temp.status = 1;
    // pthread_cond_t readyCond;
   	// bRunnable =0;
   	// pthread_mutex_t	readyMutex;
//pthread_t c;
   WrapperArg wrapperArg;
   wrapperArg.funcPtr = start_routine;
   wrapperArg.funcArg = arg;

  pthread_create(thread,attr,__wrapperFunc,&wrapperArg);//제어의 새로운 흐름을 만듦
                  //sleep(1);
  
sleep(1);
        pthread_kill(*thread,SIGUSR1);//보내주자

           	//printf("\ntest test : %d\n",*thread);

}


int 	thread_join(thread_t thread, void **retval)//이게 머지
{
    return pthread_join(thread,retval);
}
int thread_exit(void* retval)
{

}

int 	thread_suspend(thread_t tid)
{
  Ready_remove_element(getThread(tid));
  Wait_enqueue(tid);
}


int	thread_resume(thread_t tid)
{
  Wait_remove_element(getThread_wait(tid));
  Ready_enqueue(tid);
}




thread_t	thread_self()
{
    return pthread_self();
}

//추가함
void __thread_wait_handler(int signo)
{
   Thread* pTh;
      // __getThread()는 tid로 linked list의 TCB를 찾아서 반환한다.
   pTh = getThread(pthread_self());// child에서 TCB가 초기화 안되었는데, 이 함수가 호출되어도 되나 ?
   pthread_mutex_lock(&(pTh->readyMutex));
   while (pTh->bRunnable == FALSE)
      pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
   pthread_mutex_unlock(&(pTh->readyMutex));
}

void thread_wakeup(Thread* pTh)
{
   pthread_mutex_lock(&(pTh->readyMutex));
   pTh->bRunnable = TRUE;
   pthread_cond_signal(&(pTh->readyCond));
   pthread_mutex_unlock(&(pTh->readyMutex));
}

int Ready_enqueue(thread_t i)
{   
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = 1;
  //   }
  if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = 1;
    }
      else
    {
      p->status=THREAD_STATUS_READY;//status
      p->tid = i;
      p->parentTid = pthread_self();
      p->pPrev = p->pNext = NULL;
      p->readyCond = bcond;
      p->readyMutex = bmutex;
      ReadyQHead = ReadyQTail = p;
      ret = 0;
    }
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
      ret = 1;
    }
  else
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = 1;
    }
      else
    {
      p->tid = i;
      p->parentTid = pthread_self();

      p->status=THREAD_STATUS_READY;//status
      p->pPrev = p->pNext = NULL;
      p->readyCond = bcond;
      p->readyMutex = bmutex;
      ReadyQTail->pNext = p;
      p->pPrev = ReadyQTail;
      ReadyQTail = p;
      ret = 0;
    }
    }
 
  return ret;
}

int Ready_dequeue()
{
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = 1;
  //   }
  if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      printf("Nothing to Dequeue()\n");
      ret = 0;
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
      ret = 1;
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
      ret = 0;
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
      printf("Nothing to ReadyQ\n");
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = ReadyQHead;
      printf("ReadyQoutput\n");      
      while(p)
    {
      printf("tid : %u parent id : %u status: %d\n", (unsigned int)p->tid,(unsigned int)p->parentTid,p->status);
      p = p->pNext;
    }
    
    }
}
int Wait_enqueue(thread_t i)
{   
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = 1;
  //   }
  if(NULL == WaitQHead && NULL == WaitQTail)
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = 1;
    }
      else
    {
      p->status=THREAD_STATUS_BLOCKED;//status
      p->tid = i;
      p->parentTid = pthread_self();
      p->pPrev = p->pNext = NULL;
      p->readyCond = bcond;
      p->readyMutex = bmutex;
      p->bRunnable =0;
      WaitQHead = WaitQTail = p;
      ret = 0;
    }
    }
  else if(NULL == WaitQHead || NULL == WaitQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
      ret = 1;
    }
  else
    {
      struct _Thread* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = 1;
    }
      else
    {
      p->tid = i;
      p->parentTid = pthread_self();
      p->status=THREAD_STATUS_BLOCKED;//status
      p->pPrev = p->pNext = NULL;
      p->readyCond = bcond;
      p->readyMutex = bmutex;
      p->bRunnable =0;
      WaitQTail->pNext = p;
      p->pPrev = WaitQTail;
      WaitQTail = p;
      ret = 0;
    }
    }
 
  return ret;
}

int Wait_dequeue()
{
  int ret;
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //     ret = 1;
  //   }
  if(NULL == WaitQHead && NULL == WaitQTail)
    {
      printf("Nothing to Dequeue()\n");
      ret = 0;
    }
  else if(NULL == WaitQHead || NULL == WaitQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
      ret = 1;
    }
  else
    {
      struct _Thread* p = WaitQHead;
      if(NULL == WaitQHead->pNext && NULL == WaitQTail->pNext) /* if last element */
    {
      WaitQHead = WaitQTail = NULL;
    }
      else
    {
      WaitQHead = WaitQHead->pNext;
       //WaitQHead->pNext->pPrev=NULL;//형린이랑 추가함
    }

      free(p);
      ret = 0;
    }
 
  return ret;
}
void Wait_print_queue()
{
  // if(NULL == s)
  //   {
  //     fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
  //    }
  if(NULL == WaitQHead && NULL == WaitQTail)
    {
      printf("Nothing to WaitQ\n");
    }
  else if(NULL == WaitQHead || NULL == WaitQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = WaitQHead;
      printf("WaitQoutput\n");      
      while(p)
    {
      printf("tid : %u parent id : %u status: %d\n", (unsigned int)p->tid,(unsigned int)p->parentTid,p->status);
      p = p->pNext;
    }
    
    }
}
Thread* getThread(thread_t i)
{
    if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
      printf("Nothing to\n");
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = ReadyQHead;
      //printf("ReadyQoutput\n");      
      for(; p; p = p->pNext)
    {
      if(i == p->tid)
        {

          return p;
        }
    }
    return NULL;//실패?
    
    }
}
Thread* getThread_wait(thread_t i)
{
    if(NULL == WaitQHead && NULL == WaitQTail)
    {
      printf("Nothing to\n");
    }
  else if(NULL == WaitQHead || NULL == WaitQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
    }
  else
    {
      struct _Thread* p = WaitQHead;
      //printf("ReadyQoutput\n");      
      for(; p; p = p->pNext)
    {
      if(i == p->tid)
        {
          return p;
        }
    }
    return NULL;//실패?
    
    }
}
void Ready_remove_element(struct _Thread* d)
{
  if(NULL == d->pNext && (NULL == ReadyQHead->pNext && NULL == ReadyQHead->pNext)) /* only one element in queue */
    {
      ReadyQHead = ReadyQHead = NULL;
    }
  else if((NULL == d->pNext) && d->pPrev) /* removing pTail */
    {
     ReadyQTail = d->pPrev;
      d->pPrev->pNext = NULL;
    }
  else if(d->pNext && (NULL == d->pPrev)) /* removing ReadyQHead */
    {
      ReadyQHead = d->pNext;
      ReadyQHead->pPrev = NULL;
    }
  else /* removing from center or somewhere */
    {
      d->pPrev->pNext = d->pNext;
      d->pNext->pPrev = d->pPrev;
    }
 
  free(d);
}
void Wait_remove_element(struct _Thread* d)
{
  if(NULL == d->pNext && (NULL == WaitQHead->pNext && NULL == WaitQHead->pNext)) /* only one element in queue */
    {
      WaitQHead = WaitQHead = NULL;
    }
  else if((NULL == d->pNext) && d->pPrev) /* removing pTail */
    {
     WaitQHead = d->pPrev;
      d->pPrev->pNext = NULL;
    }
  else if(d->pNext && (NULL == d->pPrev)) /* removing WaitQHead */
    {
      WaitQHead = d->pNext;
      WaitQHead->pPrev = NULL;
    }
  else /* removing from center or somewhere */
    {
      d->pPrev->pNext = d->pNext;
      d->pNext->pPrev = d->pPrev;
    }
 
  free(d);
}
thread_t Ready_peek()
{
        struct _Thread* p= ReadyQHead;
        return p->tid;
}