//포인터로 구현한 큐
#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
typedef struct LinkedQueueNodeType
{
	int data;
	struct LinkedQueueNodeType* pLink;
} LinkedQueueNode;

typedef struct LinkedQueueType
{
	int currentCount;   // 현재 노드의 개수
	LinkedQueueNode* pFront;  // Front 노드의 포인터
	LinkedQueueNode* pRear;   // Rear 노드의 포인터
} LinkedQueue;

int isLinkedQueueEmpty(LinkedQueue* pQueue);

LinkedQueue* createLinkedQueue()
{
	LinkedQueue *pReturn = (LinkedQueue *)malloc(sizeof(LinkedQueue));
	memset(pReturn, 0, sizeof(LinkedQueue));

	return pReturn;
}

int enqueueLQ(LinkedQueue* pQueue, int data)
{
	int ret = 0;
	LinkedQueueNode *pNode = NULL;

	pNode = (LinkedQueueNode *)malloc(sizeof(LinkedQueueNode));
	pNode->data = data;
	pNode->pLink = NULL;

	if (isLinkedQueueEmpty(pQueue) == 0) {
		pQueue->pRear->pLink = pNode;
		pQueue->pRear = pNode;
	}
	else {
		pQueue->pFront = pNode;
		pQueue->pRear = pNode;
	}
	pQueue->currentCount++;
	ret = 1;

	return ret;
}

LinkedQueueNode* dequeueLQ(LinkedQueue* pQueue)
{
	LinkedQueueNode* pReturn = NULL;
	if (isLinkedQueueEmpty(pQueue) == 0) {
		pReturn = pQueue->pFront;
		pQueue->pFront = pQueue->pFront->pLink;
		pReturn->pLink = NULL;

		pQueue->currentCount--;
		if (isLinkedQueueEmpty(pQueue) == 1) {
			pQueue->pRear = NULL;
		}
	}

	return pReturn;
}

LinkedQueueNode* peekLQ(LinkedQueue* pQueue)
{
	LinkedQueueNode* pReturn = NULL;
	if (pQueue != NULL) {
		if (isLinkedQueueEmpty(pQueue) == 0) {
			pReturn = pQueue->pFront;
		}
	}
	return pReturn;
}

void deleteLinkedQueue(LinkedQueue* pQueue)
{
	LinkedQueueNode *pNode = NULL;
	LinkedQueueNode *pDelNode = NULL;

	if (pQueue != NULL) {
		pNode = pQueue->pFront;
		while (pNode != NULL) {
			pDelNode = pNode;
			pNode = pNode->pLink;
			free(pDelNode);
		}
		free(pQueue);
	}
}

int isLinkedQueueEmpty(LinkedQueue* pQueue)
{
	int ret = 0;

	if (pQueue != NULL) {
		if (pQueue->currentCount == 0) {
			ret = 1;
		}
	}
	return ret;
}

void displayLinkedQueue(LinkedQueue *pQueue)
{
	LinkedQueueNode *pNode = NULL;
	int i = 0;
	if (pQueue != NULL) {
		printf("현재 노드 개수: %d\n",
			pQueue->currentCount);
		pNode = pQueue->pFront;
		while (pNode != NULL) {
			printf("[%d]-[%d]\n", i, pNode->data);
			i++;
			pNode = pNode->pLink;
		}
	}
}

int main(int argc, char *argv[])
{
	LinkedQueue *pQueue = NULL;
	LinkedQueueNode *pNode = NULL;

	// 배열 큐 생성
	pQueue = createLinkedQueue();
	if (pQueue != NULL) {
		// 큐 초기화: 'A', 'B', 'C', 'D' 추가.
		enqueueLQ(pQueue, 1);
		enqueueLQ(pQueue, 2);
		enqueueLQ(pQueue, 3);
		enqueueLQ(pQueue, 4);
		displayLinkedQueue(pQueue);

		pNode = dequeueLQ(pQueue);
		if (pNode != NULL) {
			printf("Dequeue Value-[%d]\n", pNode->data);
			free(pNode);
		}
		displayLinkedQueue(pQueue);

		pNode = peekLQ(pQueue);
		if (pNode != NULL) {
			printf("Peek Value-[%d]\n", pNode->data);
		}
		displayLinkedQueue(pQueue);

		// 큐에 'E', 'F' 추가.
		enqueueLQ(pQueue, 5);

		displayLinkedQueue(pQueue);
	}
	return 0;
}
//%c로 되긴 되는데 왜 되는지는 모르겠다.
//수정함