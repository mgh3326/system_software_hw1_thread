#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <unistd.h>
#include <sys/types.h>

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
    Thread temp;
    thread_t tid=thread_self();
    //temp.status = 1;
    // pthread_cond_t readyCond;
   	// bRunnable =0;
   	// pthread_mutex_t	readyMutex;
       	Ready_enqueue(tid);
    return pthread_create(thread,attr,start_routine,arg);//제어의 새로운 흐름을 만듦
}


int 	thread_join(thread_t thread, void **retval)
{
    return pthread_join(thread,retval);
}


int 	thread_suspend(thread_t tid)
{
    Ready_dequeue();
}


int	thread_resume(thread_t tid)
{

}




thread_t	thread_self()
{
    return pthread_self();
}	
//추가함
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
      p->status=0;
      p->tid = i;
      p->pPrev = p->pNext = NULL;
 
      ReadyQHead = ReadyQTail = p;
      ret = 0;
    }
    }
  else if(NULL == ReadyQHead || NULL == ReadyQTail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
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
      p->pPrev = p->pNext = NULL;
 
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
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
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