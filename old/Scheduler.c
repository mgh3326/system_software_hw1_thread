#include "Init.h"
#include "Thread.h"
#include "Scheduler.h"
#include "doubleLinked.h"
#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<errno.h>

extern thread_t thread_head();
extern int countl; 
extern int currpid;
void _ContextSwitch(int curpid, int tpid);


void signal_handler() {	
	if(countl>2) {
		if(!isEmpty_R())
	_ContextSwitch(currpid,thread_head());
		else 
			{}
	}
}

int RunScheduler(void)
{	
	signal(SIGALRM,signal_handler);	
	while(1)
	{
	alarm(1);
	pause();
	}
}

void	_ContextSwitch(int curpid, int tpid)
{
	kill(currpid,SIGSTOP);
	list_tail_insert_R(currpid);
	kill(tpid,SIGCONT);
	currpid = tpid;	
	list_head_remove_R();
}







