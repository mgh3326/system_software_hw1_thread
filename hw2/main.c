#include "Init.h"
#include "Scheduler.h"
#include "Thread.h"

#include<stdio.h>

typedef thread_t thread_id;//추가함
int main(void)
{
	 thread_id tid;//thread_t아닌가?
     int arg;
     Init();

     thread_create(&tid, NULL, AppTask, &arg);

    RunScheduler();
     //여기부터 추가
     printf("test\n");
}

