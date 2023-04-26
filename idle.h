#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h"
#include "pcb.h"
#include "system.h"

class Idle : public Thread{
public:
	Idle();
	void run();
	void start();

	friend class System;
	friend class Thread;
};


#endif

