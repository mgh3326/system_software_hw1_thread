#include "Init.h"
#include "Thread.h"
#include "my.h"
#include <stdlib.h>
pthread_cond_t run_wait;

pthread_mutex_t run_lock;
void Init(void)
{
    pthread_mutex_init(&run_lock, NULL);
	pthread_cond_init(&run_wait, NULL);
}
