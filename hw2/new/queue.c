#include <stdlib.h>
#include "queue.h"

struct node;
typedef struct node Node;

typedef struct node
{
	int size;
	Node* next;
	Node* previous;
}Node;

typedef struct queue
{
	Node* pHead;
	Node* pTail;
}Queue;

typedef struct queue Queue;


QUEUE queue_init_default(void)
{
	Queue* pQueue;

	pQueue = (Queue*)malloc(sizeof(Queue));
	if (pQueue != NULL)
	{
		pQueue->pHead = NULL;
		pQueue->pTail = NULL;
	}
	return pQueue;
}

Status queue_enqueue(QUEUE hQueue, int size)
{
	Queue* pQueue = (Queue*)hQueue;
	Node* temp;
	temp = (Node*)malloc(sizeof(Node));
	if (temp == NULL)
		return FAILURE;

	temp->size = size;
	temp->next = NULL;
	if(pQueue != NULL){
		if (pQueue->pTail != NULL) {
			pQueue->pTail->next = temp;
		}
		temp->previous = pQueue->pTail;
		pQueue->pTail = temp;
		if (pQueue->pHead == NULL)
			pQueue->pHead = temp;
	}
	return SUCCESS;
}

Status queue_service(QUEUE hQueue)
{
	Queue* pQueue = (Queue*)hQueue;
	Node* temp;

	if (queue_empty(hQueue)) {
		return FAILURE;
	}
	if (pQueue->pHead->next == NULL) {
		free(pQueue->pHead);
		pQueue->pHead = NULL;
		return SUCCESS;
	}
	temp = pQueue->pHead->next;
	free(pQueue->pHead);
	pQueue->pHead = temp;

	return SUCCESS;
}	

int queue_front(QUEUE hQueue, Status* pStatus)
{
	Queue* pQueue = (Queue*)hQueue;
	if (queue_empty(hQueue)) {
		if (pStatus != NULL)
		{
			*pStatus = FAILURE;
		}
		return -8887888;
	}
	if (pStatus != NULL)
	{
		*pStatus = SUCCESS;
	}
	return pQueue->pHead->size;
}

Boolean queue_empty(QUEUE hQueue)
{
	Queue* pQueue = (Queue*)hQueue;

	return (Boolean)(pQueue->pHead == NULL);
}

void queue_destroy(QUEUE* phQueue)
{
	Queue* pQueue = (Queue*)*phQueue;
	Node* temp;

	while (pQueue->pHead != NULL) 
	{
		temp = pQueue->pHead;
		pQueue->pHead = pQueue->pHead->next;
		free(temp);
	}
	free(pQueue->pHead);
	pQueue->pHead = NULL;
	*phQueue = NULL;
	return;
}