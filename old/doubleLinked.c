#include<stdio.h>
#include<stdlib.h>
#include"Thread.h"
#include"Scheduler.h"
#include"doubleLinked.h"

int countl =0;
Thread* GetNewNode_R(pid_t t) {
	Thread *new = (Thread *)malloc(1*sizeof(Thread));
	new->status =1;
	new->pid = t;
	new->pPrev = NULL;
	new->pNext = NULL;
	return new;
}

int isEmpty_R() { 
	if(ReadyQHead == NULL) 
		return 1;
	else
		return 0;
}

void list_tail_insert_R(pid_t t) {
	Thread *temp = ReadyQHead;
	Thread *temp2 = GetNewNode_R(t);
	if(ReadyQHead == NULL) {
		ReadyQHead = temp2;
		return;
	}
	while(temp->pNext != NULL) temp=temp->pNext;
	temp->pNext = temp2;
	temp2->pPrev = temp;
	countl++;
}

void dis_R() {
		printf("This is Ready Queue\n");
	Thread *temp = ReadyQHead;
	while(temp!=NULL) {
 		printf("pid  = %d",temp->pid);
		printf(" status = %d\n",temp->status);
		temp=temp->pNext;
	}
}

void list_head_remove_R() {
	Thread *temp2;
	int i =1; int pos =1;
	temp2 = ReadyQHead;
	while(i<pos) {
	temp2 = temp2 -> pNext;
	i++;
	}
	if(i==1) {
		if(temp2->pNext == NULL) {
			free(temp2);
			temp2 = ReadyQHead = NULL;
			return ;
		}
	if(temp2->pNext == NULL) {
		temp2->pPrev->pNext = NULL;
		free(temp2);
		return ;
	}
	temp2->pNext->pPrev = temp2->pPrev;
	if(i==1)
		ReadyQHead = temp2->pNext;
	free(temp2);
	}
	countl--;
}
Thread* list_remove_specific_R(pid_t t) {
	Thread *current = ReadyQHead;
	Thread *previous = NULL;
	
	if(ReadyQHead == NULL) {
		return NULL;
	}
	while(current->pid != t) {
		if(current->pNext == NULL) {
			return 0;
		}
		else {
			previous = current;
		current = current ->pNext;
		}
	}
	if(current == ReadyQHead) {
	ReadyQHead = ReadyQHead -> pNext;	
	countl--;	
	}
	else {
	current->pPrev->pNext = current -> pNext;	
	countl--;
	}
	return current;
}
// ready queue


// waiting queue
Thread* GetNewNode_W(pid_t t) {
	Thread *new = (Thread *)malloc(1*sizeof(Thread));
	new->status =2;
	new->pid = t;
	new->pPrev = NULL;
	new->pNext = NULL;
	return new;
}

void list_tail_insert_W(pid_t t) {
	Thread *temp = WaitQHead;
	Thread *temp2 = GetNewNode_W(t);
	if(WaitQHead == NULL) {
		WaitQHead = temp2;
		return;
	}
	while(temp->pNext != NULL) temp=temp->pNext;
	temp->pNext = temp2;
	temp2->pPrev = temp;
}

void dis_W() {
		printf("This is Waitting Queue\n");
	Thread *temp = WaitQHead;
	while(temp!=NULL) {
 		printf("pid  = %d",temp->pid);
		printf(" status = %d\n",temp->status);
		temp=temp->pNext;
	}
}

void list_head_remove_W() {
	Thread *temp2;
	int i =1; int pos =1;
	temp2 = WaitQHead;
	while(i<pos) {
	temp2 = temp2 -> pNext;
	i++;
	}
	if(i==1) {
		if(temp2->pNext == NULL) {
			free(temp2);
			temp2 = WaitQHead = NULL;
			return ;
		}
	if(temp2->pNext == NULL) {
		temp2->pPrev->pNext = NULL;
		free(temp2);
		return ;
	}
	temp2->pNext->pPrev = temp2->pPrev;
	if(i==1)
		WaitQHead = temp2->pNext;
	free(temp2);
	}
}

Thread* list_remove_specific_W(pid_t t) {
	Thread *current = WaitQHead;
	Thread *previous = NULL;
	
	if(WaitQHead == NULL) {
		return NULL;
	}
	while(current->pid != t) {
		if(current->pNext == NULL) {
			return 0;
		}
		else {
			previous = current;
		current = current ->pNext;
		}
	}
	if(current == WaitQHead) {
	WaitQHead = WaitQHead -> pNext;
	}
	else {
	current->pPrev->pNext = current -> pNext;
	}
	return current;
}
//waiting queue


