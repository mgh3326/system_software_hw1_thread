#ifndef __DOUBLELINKED_H__
#define __DOUBLELINED_H__
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

Thread* GetNewNode_R(pid_t t);
void list_tail_insert_R(pid_t t);
void dis_R();
int isEmpty_R();
void list_head_remove_R();
Thread* list_remove_specific_R(pid_t t);

Thread* GetNewNode_W(pid_t t);
void list_tail_insert_W(pid_t t);
void dis_W();
void list_head_remove_W();
Thread* list_remove_specific_W(pid_t t);

#endif

