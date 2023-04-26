#ifndef PCB_H_
#define PCB_H_

#include "queue.h"
#include "thread.h"

class Queue;

class PCB {
public:
	PCB(Thread* myThread, Time timeSlice, StackSize stackSize);
	~PCB();
	void newStack();

	Thread* myThread;
	enum State {NEW, READY, BLOCKED, DONE};
	State myState;
	unsigned int sp,ss;
	unsigned* SP;
	int waitReturn;

	Time passedTime, myTimeSlice, semSleepTime;
	StackSize myStackSize;
	Queue* incompleteQueue;

private:
    ID myID;
    static ID globalID;

    friend class System;
    friend class Thread;
    friend class Idle;
};

#endif
