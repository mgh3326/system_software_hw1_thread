#ifndef __THREAD_H__
#define __THREAD_H__
#include <stdlib.h>

#include <pthread.h>
#include <stdbool.h>
#include <signal.h>

#define TIMESLICE	(2)


typedef int BOOL;
typedef pthread_t thread_t;
typedef void thread_attr_t;


typedef enum{
	THREAD_STATUS_RUN = 0,
	THREAD_STATUS_READY = 1,
	THREAD_STATUS_BLOCKED = 2,
	THREAD_STATUS_ZOMBIE = 3
}ThreadStatus;

typedef struct _Thread Thread;
typedef struct _Thread {
	ThreadStatus status;
    pthread_t tid;
    pthread_cond_t readyCond;
   	BOOL bRunnable;
   	pthread_mutex_t	readyMutex;
	pthread_t parentTid;

	Thread*	pPrev;
	Thread*	pNext;
} Thread;	

typedef struct __wrapperArg {
   void*  (*funcPtr)(void*);
   void* funcArg;
} WrapperArg;



/* head and tail pointers for ready queue */ 
Thread* 	ReadyQHead;
Thread*		ReadyQTail;

/* head and tail pointers for waiting queue */
Thread*		WaitQHead;
Thread*		WaitQTail;

Thread* 	Running_Thread;

void* __wrapperFunc(void* arg);
int thread_create(thread_t *thread, thread_attr_t *attr, void *(*start_routine) (void *), void *arg);
int thread_join(thread_t thread, void **retval);
int thread_exit(void* retval);
int thread_suspend(thread_t tid);//ready -> wait
int	thread_resume(thread_t tid);//wait -> ready
thread_t thread_self();
//여기 부터 추가
void thread_wait(thread_t tid);
int Ready_enqueue(pthread_t i);
int Ready_dequeue();
void Ready_print_queue();
int Wait_enqueue(pthread_t i);
int Wait_dequeue();
void Wait_print_queue();
Thread* getThread(thread_t i);
void Ready_remove_element(struct _Thread* d);
Thread* getThread_wait(thread_t i);
void Wait_remove_element(struct _Thread* d);
void __thread_wait_handler(int signo);
void __thread_wakeup(Thread* pTh);
thread_t Ready_peek();
//여기 까지 추가
#endif /* __THREAD_H__ */