#include "Init.h"
#include "Thread.h"
#include "my.h"
#include <stdlib.h>

void Init(void)
{
	sign = 0;
	pthread_mutex_init(&run_lock, NULL);
	pthread_cond_init(&run_wait, NULL);
}
