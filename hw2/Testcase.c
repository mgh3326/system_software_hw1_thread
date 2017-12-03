 
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <errno.h>
#include "Thread.h"

#define TOTAL_THREAD_NUM (5)
int fun1(int param);
int fun2(int param);
int fun2(int param);

void Testcase(void);


extern int countl;
 
int Tc1ThreadProc(int param)
{
			//sleep(1);

	int tid = 0;
	int count = 0;

	tid = thread_self();

	count = 50;
	while (count > 0)
	{
		/* sleep for 1 seconds */
		sleep(1);
		printf("Tc1ThreadProc: my thread id (%u) count is (%d)\n",thread_self(), count);
				sleep(1);


		count--;
	}
}

/* 
 * - TestCase1 tests a round-robin scheduling of thread  
 * - Testing API scopes: thread_create
 */
void Testcase(void)
{
	thread_t tid[TOTAL_THREAD_NUM];
	
	thread_create(&tid[0], NULL, (void*)Tc1ThreadProc, (int*)1);
	thread_create(&tid[1], NULL, (void*)Tc1ThreadProc, (int*)2);
	thread_create(&tid[2], NULL, (void*)Tc1ThreadProc, (int*)3);
	// thread_create(&tid[3], NULL, (void*)Tc1ThreadProc, (int*)4);
	// thread_create(&tid[4], NULL, (void*)Tc1ThreadProc, (int*)5);

	while(1){}
	
	return ;
}

