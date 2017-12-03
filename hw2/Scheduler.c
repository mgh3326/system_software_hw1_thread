#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include <unistd.h>
#include <stdio.h>
static pthread_cond_t bcond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t bmutex = PTHREAD_MUTEX_INITIALIZER;
int RunScheduler( void )
{
        //printf("test");
        //sleep(TIMESLICE);
    // while(1)
    // {
    // sleep(TIMESLICE);
    // if(NULL == ReadyQHead && NULL == ReadyQTail)
    // continue;
    //                 sleep(TIMESLICE);
    // break;
    // }
    struct _Thread* p = malloc(sizeof(Thread));

    thread_t run_tid=0;
    thread_t current_tid=0;
        thread_t temp_tid=0;

    int i=0;
    Thread* tp;
    while(1)
    {

    //레디큐가 비었는지 먼저 확인
    if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
    sleep(TIMESLICE);
        continue;
    }
    if(i==0)
    {
        //sleep(2);
        //printf("ReadyQHead->tid : ,%u   test2 : %u\n",ReadyQHead->tid,getThread(ReadyQHead->tid)->tid);
        
    run_tid=ReadyQHead->tid;
    Running_Thread =p;
    Running_Thread->tid=ReadyQHead->tid;
    Running_Thread->parentTid = pthread_self();
      Running_Thread->pPrev = Running_Thread->pNext = NULL;
      Running_Thread->readyCond = ReadyQHead->readyCond;
      Running_Thread->readyMutex = ReadyQHead->readyMutex;
    Running_Thread->status=THREAD_STATUS_RUN;//status
    Running_Thread->bRunnable =1;

    //sleep(TIMESLICE);
    __thread_wakeup(ReadyQHead);//깨우고
    sleep(TIMESLICE);

    //printf("tp->tid : %u\n",tp->tid);
    i++;
    }
    //printf("test : %u\n",Running_Thread->tid);
        // run_tid=Ready_peek();
        // Thread* rp =getThread(run_tid);
        
        //printf("ReadyQHead->tid : ,%u   test2 : %u\n",ReadyQHead->tid,getThread(ReadyQHead->tid)->tid);
    //thread_wait(Running_Thread->tid);
      //pthread_kill(ReadyQHead->tid, SIGUSR1);
    pthread_kill(Ready_peek()->tid, SIGUSR1);
Running_Thread->tid=ReadyQHead->tid;
    Running_Thread->parentTid = pthread_self();
      Running_Thread->pPrev = Running_Thread->pNext = NULL;
      Running_Thread->readyCond = ReadyQHead->readyCond;
      Running_Thread->readyMutex = ReadyQHead->readyMutex;
    Running_Thread->status=THREAD_STATUS_RUN;//status
    Running_Thread->bRunnable =1;
            pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고

            Ready_dequeue();
            
    ///Thread* rp =Ready_peek();
    //Ready_dequeue();
        //printf("run_tid : %u current_tid : %u \n",(unsigned int)Running_Thread->tid,(unsigned int)rp->tid);
        pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고

    Ready_enqueue(Running_Thread->tid);
        Running_Thread->bRunnable =0;
        ReadyQTail->bRunnable=0;
    //thread_create(Running_Thread->tid,NULL,NULL,NULL);
    pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고
    //current_tid=rp->tid;

    Running_Thread->tid=Ready_peek()->tid;
Running_Thread->bRunnable =0;
    //__thread_wakeup(Running_Thread);
        __thread_wakeup(Ready_peek());//깨우고

    sleep(TIMESLICE);
          //pthread_kill(rp->tid, SIGUSR1);

}
}
void __ContextSwitch(Thread pCurThread, Thread* pNewThread)
{

}