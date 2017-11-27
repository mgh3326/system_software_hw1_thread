#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"

#include<stdio.h>
#include<unistd.h>
void* AppTask();
typedef thread_t thread_id;//추가함
int main(void)
{
	 thread_id tid1,tid2,tid3,tid4;//thread_t아닌가?
     int arg;
    //  Init();
    ReadyQHead =NULL;
    ReadyQTail=NULL;
    
    thread_create(&tid1, NULL, AppTask, &arg);
    sleep(1);
    Ready_print_queue();
    
    thread_create(&tid2, NULL, AppTask, &arg);
    sleep(1);
    Ready_print_queue();

    thread_create(&tid3, NULL, AppTask, &arg);
    sleep(1);
    Ready_print_queue();

    thread_create(&tid4, NULL, AppTask, &arg);
    sleep(1);
    Ready_print_queue();
    
    // for(int i=0;i<10;i++)
    // {
    //     Ready_enqueue(i);
    //     Ready_print_queue();
    // }
    // for(int i=0;i<10;i++)
    // {
    //     Ready_dequeue();
    //     Ready_print_queue();

    // }
    // Ready_print_queue();
    RunScheduler();
     //여기부터 추가
     //printf("test\n");
}
void* AppTask(void *arg)
{
    while(1);
    return((void *)0);

}