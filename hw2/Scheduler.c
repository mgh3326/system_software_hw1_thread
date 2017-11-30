#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include <unistd.h>
#include <stdio.h>
int RunScheduler( void )
{
        printf("test");
        //sleep(TIMESLICE);

    thread_t run_tid=0;
    thread_t current_tid=0;
    while(1)
    {
                sleep(TIMESLICE);

    //레디큐가 비었는지 먼저 확인
    if(NULL == ReadyQHead && NULL == ReadyQTail)
    {
        continue;
    }
    
        run_tid=Ready_peek();
        Thread* rp =getThread(run_tid);
            Ready_dequeue();
    current_tid=Ready_peek();
    Thread* p =getThread(current_tid);
    
    //Ready_dequeue();
        printf("run_tid : %u current_tid : %u \n",(unsigned int)run_tid,(unsigned int)current_tid);
    Ready_enqueue(run_tid);
    //sleep(TIMESLICE);
    }
}


void __ContextSwitch(Thread pCurThread, Thread* pNewThread)
{

}

