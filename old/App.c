#include<stdio.h>

extern void TestCase1();
void* App_Task(void *arg) {
	TestCase1();
	return NULL;
}
