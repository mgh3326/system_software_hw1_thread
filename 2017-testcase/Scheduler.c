#include "Init.h"
#include "Thread.h"
#include "my.h"
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
    // struct _Thread* p = malloc(sizeof(Thread));

    // thread_t run_tid=0;
    // thread_t current_tid=0;
    //     thread_t temp_tid=0;

    int i=0;
    // Thread* tp;
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
                sleep(TIMESLICE);

        //sleep(TIMESLICE);
        
        //sleep(2);
        //printf("ReadyQHead->tid : ,%u   test2 : %u\n",ReadyQHead->tid,getThread(ReadyQHead->tid)->tid);
        
    // run_tid=ReadyQHead->tid;
    // Running_Thread =p;
    // Running_Thread->tid=ReadyQHead->tid;
    // Running_Thread->parentTid = pthread_self();
    //   Running_Thread->pPrev = Running_Thread->pNext = NULL;
    //   Running_Thread->readyCond = ReadyQHead->readyCond;
    //   Running_Thread->readyMutex = ReadyQHead->readyMutex;
    // Running_Thread->status=THREAD_STATUS_RUN;//status
    // Running_Thread->bRunnable =1;
    
    //sleep(TIMESLICE);
    //ReadyQHead->bRunnable=0;
    __thread_wakeup(ReadyQHead);//깨우고
    ReadyQHead->status=THREAD_STATUS_RUN;
    
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
    //pthread_kill(Ready_peek()->tid, SIGUSR1);
// Running_Thread->tid=ReadyQHead->tid;
//     Running_Thread->parentTid = pthread_self();
//       Running_Thread->pPrev = Running_Thread->pNext = NULL;
//       Running_Thread->readyCond = ReadyQHead->readyCond;
//       Running_Thread->readyMutex = ReadyQHead->readyMutex;
//     Running_Thread->status=THREAD_STATUS_RUN;//status
//     Running_Thread->bRunnable =1;
            //pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고
            //ReadyQHead->bRunnable=0;
    			
              ReadyQHead->bRunnable=0;

            Ready_dequeue();//자리를 바꿈

            //printf("test : %d\n",ReadyQTail);

    ///Thread* rp =Ready_peek();
    //Ready_dequeue();
        //printf("run_tid : %u current_tid : %u \n",(unsigned int)Running_Thread->tid,(unsigned int)rp->tid);
        //pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고

    //Ready_enqueue(Running_Thread->tid);
        //ReadyQTail->bRunnable=0;
    //thread_create(Running_Thread->tid,NULL,NULL,NULL);
    // pthread_kill(Running_Thread->tid, SIGUSR1);//잠재우고
    // __thread_wakeup(Ready_peek());//깨우고

    __ContextSwitch(ReadyQTail,ReadyQHead);
//if(ReadyQTail->status==THREAD_STATUS_ZOMBIE)
        //Ready_remove_element(ReadyQTail);


    
    //Running_Thread->tid=Ready_peek()->tid;
    //__thread_wakeup(Running_Thread);
    Ready_print_queue();
    Wait_print_queue();

    sleep(TIMESLICE);


          //pthread_kill(rp->tid, SIGUSR1);

}
}
void __ContextSwitch(Thread* pCurThread, Thread* pNewThread)
{    //pCurThread->bRunnable=THREAD_STATUS_RUN;
    if(pCurThread==NULL||pNewThread==NULL)
    {
                    // pthread_kill(pCurThread->tid, SIGUSR2);

        		sleep(2);

        return;
    }
    
    //pNewThread->bRunnable=THREAD_STATUS_READY;
 
        if(pCurThread->status==THREAD_STATUS_BLOCKED)
        {

        Ready_remove_element(pCurThread);
        //pCurThread->pPrev->pNext=NULL;

    }
    else{

    
    __thread_wakeup(pNewThread);
    if(pCurThread==NULL||pNewThread==NULL)
    {
            // pthread_kill(pCurThread->tid, SIGUSR2);

        		sleep(2);

        return;
    }
    
    pthread_kill(pCurThread->tid, SIGUSR1);
    pNewThread->status=THREAD_STATUS_RUN;
        if(pCurThread->status==THREAD_STATUS_RUN)

    if(pCurThread->status==THREAD_STATUS_RUN)
    pCurThread->status=THREAD_STATUS_READY;
        }
    //pNewThread->bRunnable=THREAD_STATUS_RUN;
    
    
}