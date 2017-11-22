#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
// int done = 0;

// pthread_mutex_t m =  
//     PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t c = 
//     PTHREAD_COND_INITIALIZER;
void mythread_exit();
void mythread_join();

int done = 0;
pthread_mutex_t m =  
    PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = 
    PTHREAD_COND_INITIALIZER;
void *child(void *arg) {
   printf("child\n");
   mythread_exit();
   return NULL;
}
int main(int argc, char *argv[])
{
   pthread_t c;
   printf("parent: begin\n");  
   mythread_join();
   printf("parent: end\n");
   return 0;
}
void mythread_exit()
{
pthread_mutex_lock(&m);
   done = 1;
   pthread_cond_signal(&c);
   pthread_mutex_unlock(&m);

}
void mythread_join()
{
    pthread_mutex_lock(&m);
   while (done == 0)
      pthread_cond_wait(&c, &m);
   pthread_mutex_unlock(&m);

}