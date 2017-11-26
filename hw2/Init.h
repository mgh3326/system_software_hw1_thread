#ifndef _INIT_H_
#define _INIT_H_
#include "Thread.h"//이렇게 추가 해주는게 맞는건가?

void Init(void);

void Init(void)
{
   
    // Create ready queue and waiting queue
    ReadyQHead =NULL;
    ReadyQTail=NULL;
    WaitQHead=NULL;
    WaitQTai=NULL;
    //…
    // initialize thread scheduler  
}
#endif


