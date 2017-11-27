/* A Queue (FIFO)  implementation using doubly-linked list. All operations are based on page 70, section 2.4 "Data Structures and 
 *   Algorithms by Aho, Hopcraft and Ullman".
 *
 * VERSION 0.0
 *
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
enum { VAL_SUCC = 0, VAL_ERR = 1};
 
struct dlqueue
{
  int num;
  struct dlqueue* next;
  struct dlqueue* prev;
};
 
 
struct dlqlist
{
  struct dlqueue* head;
  struct dlqueue* tail;
};
 
 
 
int enqueue(struct dlqlist*, int);
int dequeue(struct dlqlist*);
struct dlqueue* front(struct dlqlist*);
void makenull(struct dlqlist*);
int empty(struct dlqlist*);
void print_queue(struct dlqlist* );
 
 
int dequeue_anywhere(struct dlqlist*s , int numd);
void remove_element(struct dlqlist* s, struct dlqueue* d);
int peek(struct dlqlist* s);
 
int main(void)
{
  struct dlqlist* s = malloc(1 * sizeof *s);
  if(NULL == s)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      return EXIT_FAILURE;
    }
  else
    {
      s->head = s->tail = NULL;
    }
 
  enqueue(s, 10);
  enqueue(s, 11);
  enqueue(s, 12);
  enqueue(s, 13);
  print_queue(s);
  printf("dequeue test : %d \n",peek(s));//출력값은 성공값만 나오나보네
  dequeue(s);
  print_queue(s);
  dequeue_anywhere(s,12);
  printf("\n\n----------------------------\n");
  print_queue(s);
 
  /*
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
 
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
 
 
  dequeue(s);
  printf("\n\n----------------------------\n");
  print_queue(s);
  */
  return EXIT_SUCCESS;
}
 
 
 
/* Adds an element to tail of Queue */
int enqueue(struct dlqlist* s, int i)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      struct dlqueue* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->num = i;
      p->prev = p->next = NULL;
 
      s->head = s->tail = p;
      ret = VAL_SUCC;
    }
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct dlqueue* p = malloc(1 * sizeof *p);
      if(NULL == p)
    {
      fprintf(stderr,"IN: %s @%d: Out of Memory\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
      else
    {
      p->num = i;
      p->prev = p->next = NULL;
 
      s->tail->next = p;
      p->prev = s->tail;
      s->tail = p;
      ret = VAL_SUCC;
    }
    }
 
  return ret;
}
 
 
 
int dequeue(struct dlqlist* s)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to Dequeue()\n");
      ret = VAL_SUCC;
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct dlqueue* p = s->head;
      if(NULL == s->head->next && NULL == s->tail->next) /* if last element */
    {
      s->head = s->tail = NULL;
    }
      else
    {
      s->head = s->head->next;
    }
 
      free(p);
      ret = VAL_SUCC;
    }
 
  return ret;
}
 
 
int dequeue_anywhere(struct dlqlist*s , int numd)
{
  int ret;
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
      ret = VAL_ERR;
    }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to Dequeue()\n");
      ret = VAL_SUCC;
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
      ret = VAL_ERR;
    }
  else
    {
      struct dlqueue* p = s->head;
      for(; p; p = p->next)
    {
      if(numd == p->num)
        {
          remove_element(s,p);
        }
    }
      ret = VAL_SUCC;
    }
 
  return ret;
}
 
 
void remove_element(struct dlqlist* s, struct dlqueue* d)
{
  if(NULL == d->next && (NULL == s->head->next && NULL == s->tail->next)) /* only one element in queue */
    {
      s->head = s->tail = NULL;
    }
  else if((NULL == d->next) && d->prev) /* removing tail */
    {
      s->tail = d->prev;
      d->prev->next = NULL;
    }
  else if(d->next && (NULL == d->prev)) /* removing head */
    {
      s->head = d->next;
      s->head->prev = NULL;
    }
  else /* removing from center or somewhere */
    {
      d->prev->next = d->next;
      d->next->prev = d->prev;
    }
 
  free(d);
}
 
void remove_element_2(struct dlqlist* s, struct dlqueue* d)
{
  if(NULL == d->next)
    {
      s->tail = d->prev;
    }
  else
    {
      d->next->prev = d->prev;
    }
 
  if(NULL == d->prev)
    {
      s->head = d->next;
    }
  else
    {
      d->prev->next = d->next;
    }
 
  free(d);
 
 
}
 
 
void print_queue(struct dlqlist* s)
{
  if(NULL == s)
    {
      fprintf(stderr, "IN: %s @ %d: Invalid Args\n", __FILE__, __LINE__);
     }
  else if(NULL == s->head && NULL == s->tail)
    {
      printf("Nothing to print\n");
    }
  else if(NULL == s->head || NULL == s->tail)
    {
      fprintf(stderr, "IN: %s @%d: Serious error.", __FILE__, __LINE__);
      fprintf(stderr,"List one of the list's head/tail is null while other is not\n");
    }
  else
    {
      struct dlqueue* p = s->head;
      while(p)
    {
      printf("num = %d\n", p->num);
      p = p->next;
    }
    }
}
int peek(struct dlqlist* s)
{
        struct dlqueue* p = s->head;
        return p->num;
}