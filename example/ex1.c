#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int done = 0;

void *child(void *arg) {
   printf("child\n");
   done = 1;
   return NULL;
}

int main(int argc, char *argv[])
{
   pthread_t c;
   printf("parent: begin\n");  
   pthread_create(&c, NULL, 
      child, NULL);
   while (done == 0); // spin
   printf("parent: end\n");
   return 0;
}
