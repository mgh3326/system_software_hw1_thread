#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include "my.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/types.h>
int d=0;
#define TRUE 1
#define FALSE 0
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
//printf("ReadQ insert tid : %u\n",(unsigned int)thread_self());
  
  sigset_t set;
  int retSig;
  // child sleeps until TCB is initialized
     signal(SIGUSR1, __thread_wait_handler);//핸들러 등록
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigwait(&set, &retSig);
  // child is ready to run
  void* ret;
  WrapperArg* pArg = (WrapperArg*)arg;
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
   //WrapperArg wrapperArg;
   //  wrapperArg.funcPtr = start_routine;
  //  wrapperArg.funcArg = arg;//밑에꺼랑 무슨차이지
   WrapperArg* wrapperArg=(WrapperArg*)malloc(sizeof(WrapperArg));
  
   wrapperArg->funcPtr = start_routine;
   wrapperArg->funcArg = arg;
  pthread_create(thread,attr,__wrapperFunc,wrapperArg);//제어의 새로운 흐름을 만듦

                  // while(getThread(thread_self()) == NULL) {
                  //   //printf("not yet!\n");
                  //   }
                  usleep(1000);//usleep(1000000) 이게 1초
        pthread_kill(*thread,SIGUSR1);//보내주자

           	//printf("\ntest test : %d\n",*thread);

}


int 	thread_join(thread_t thread, void **retval)//이게 머지
{
  //Set thread status to sleep
  //Remove this thread’s TCB from ready queue
//insert TCB into waiting queue

//Call __thread_wait_handler()

  //   pthread_mutex_lock(&m);
  //  while (done == 0)
  //     pthread_cond_wait(&c, &m);
  //  pthread_mutex_unlock(&m);
//insert TCB into ready queue
//Set thread status to ready
//Get pExitCode of child’s TCB; Put pExitCode into retVal;
//Remove child’s TCB from ready queue; deallocate child’s TCB
//return
}
int thread_exit(void* retval)
{
  //Get parent’s TCB from this thread’s TCB
  //Call __thread_wakeup using Parent TCB
//set pExitCode of this thread’s TCP to exitValue

//  pthread_mutex_lock(&m);
//    while (done == 0)
//       pthread_cond_wait(&c, &m);
//    pthread_mutex_unlock(&m);

}

int 	thread_suspend(thread_t tid)
{
 
  if(getThread(tid)==NULL||getThread(tid)->bRunnable ==1)
  return -1;
  Ready_remove_element(getThread(tid));
  printf(" thread_suspend(%u)\n",(unsigned int)tid);
  Wait_enqueue(tid);
}


int	thread_resume(thread_t tid)
{
  done=0;
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
  // printf(" wait tid %u\n",pTh->tid);
   pthread_mutex_lock(&(pTh->readyMutex));
     //         printf("__thread_wait_handler start %u\n",pTh->tid);

   while (pTh->bRunnable == FALSE)
      pthread_cond_wait(&(pTh->readyCond), &(pTh->readyMutex));
   pthread_mutex_unlock(&(pTh->readyMutex));
   //printf("__thread_wait_handler end %u\n",pTh->tid);
}
void thread_wait(thread_t tid)
{
  pthread_kill(tid, SIGUSR1);
    // printf("\nthread_wait\n");

}
void __thread_wakeup(Thread* pTh)
{
   pthread_mutex_lock(&(pTh->readyMutex));
   pTh->bRunnable = TRUE;
           //printf("__thread_wakeup start%u\n",pTh->tid);

   pthread_cond_signal(&(pTh->readyCond));
   pthread_mutex_unlock(&(pTh->readyMutex));
              //printf("__thread_wakeup end%u\n",pTh->tid);

   //printf("\n__thread_wakeup\n");
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
  if(NULL == WaitQHead && NULL == WaitQTai)
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
      WaitQHead = WaitQTai = p;
      ret = 0;
    }
    }
  else if(NULL == WaitQHead || NULL == WaitQTai)
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
      WaitQTai->pNext = p;
      p->pPrev = WaitQTai;
      WaitQTai = p;
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
  if(NULL == WaitQHead && NULL == WaitQTai)
    {
      printf("Nothing to Dequeue()\n");
      ret = 0;
    }
  else if(NULL == WaitQHead || NULL == WaitQTai)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's ReadyQHead/pTail is null while other is not\n");
      ret = 1;
    }
  else
    {
      struct _Thread* p = WaitQHead;
      if(NULL == WaitQHead->pNext && NULL == WaitQTai->pNext) /* if last element */
    {
      WaitQHead = WaitQTai = NULL;
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
  if(NULL == WaitQHead && NULL == WaitQTai)
    {
      printf("Nothing to WaitQ\n");
    }
  else if(NULL == WaitQHead || NULL == WaitQTai)
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
      return NULL;
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
    if(NULL == WaitQHead && NULL == WaitQTai)
    {
      printf("Nothing to\n");
    }
  else if(NULL == WaitQHead || NULL == WaitQTai)
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
Thread* Ready_peek()
{
      return ReadyQHead;
}
thread_t thread_head()
{
	return ReadyQHead->tid;
}