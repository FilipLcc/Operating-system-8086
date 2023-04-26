#ifndef KERNELSM_H_
#define KERNELSM_H_

#include "queue.h"


class KernelSem {
public:
	KernelSem(int init=1);
	int wait(Time maxTimeToWait);
	int signal(int n=0);
	int val() const;
private:
	int value;
	Queue* waitQueue;
	Queue* timerQueue;
	friend class Semaphore;
	friend class System;
};


#endif
