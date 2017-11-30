#define _GNU_SOURCE
#include "Thread.h"
#include "Init.h"
#include "Scheduler.h"
#include"doubleLinked.h"
#include<sched.h>
#include<stdio.h>
#include<sys/types.h>
#include<wait.h>
#include<signal.h>
#include<stdlib.h>

extern int countl; 
int currpid;
int join_pid;

#define STACK_SIZE 1024*64

int 	thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	void *stack;
	stack = malloc(STACK_SIZE);
	if(stack == 0) {
		perror("malloc : could not allocate stack");
		exit(1);
	}
	*thread = clone((void *)(*start_routine), (char*)stack+STACK_SIZE,CLONE_VM|CLONE_SIGHAND|CLONE_FS|CLONE_FILES|SIGCHLD,(void*)arg);	
	if(countl==0) {
	countl++;	
	}	
	else if(countl>1) {
	kill(*thread,SIGSTOP);
	list_tail_insert_R(*thread);
	}
	else if(countl==1){
	currpid = *thread;
	countl++;
	}	
}

thread_t thread_head()
{
	return ReadyQHead->pid;
}

void signal_hand(int signo,siginfo_t *info,void *uarg);

int 	thread_join(thread_t thread, void **retval)
{
	join_pid = getpid();	
	struct sigaction act;
	act.sa_sigaction=signal_hand;
	act.sa_flags=SA_NOCLDSTOP|SA_SIGINFO;
	sigemptyset(&act.sa_mask);
	sigaction(SIGCHLD,&act,NULL);
	kill(join_pid,SIGSTOP);
	list_tail_insert_W(join_pid);
}

void signal_hand(int signo,siginfo_t *info,void *uarg) {
	kill(join_pid,SIGCONT);
	currpid = join_pid;	
	list_tail_insert_R(join_pid);
	list_remove_specific_W(join_pid);
}

int 	thread_suspend(thread_t tid)
{
	if(currpid == tid) {
		kill(tid,SIGSTOP);
		list_tail_insert_W(tid);
		if(!isEmpty_R()) {
		currpid = thread_head();
		list_head_remove_R();
		}
	}
	else {
	list_remove_specific_R(tid);
	list_tail_insert_W(tid);
	}
}

int	thread_resume(thread_t tid)
{
	if(countl<2) {
		currpid = tid;
		countl++;
		list_remove_specific_W(tid);
	}
	else {
	list_remove_specific_W(tid);
	list_tail_insert_R(tid);
	}
}

int 	thread_cancel(thread_t tid)
{
	if(currpid == tid) {
		kill(tid,SIGKILL);	
		if(!isEmpty_R()) {
		currpid = thread_head();
		kill(currpid,SIGCONT);
		list_head_remove_R();
	}	
	}
	else {
	kill(tid,SIGKILL);
	list_remove_specific_R(tid);
	list_remove_specific_W(tid);
	}
}
	
thread_t thread_self()
{
	return getpid();
}	



	
