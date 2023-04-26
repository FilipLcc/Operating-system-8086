#ifndef SEMQUEUE_H_
#define SEMQUEUE_H_


#include "kernelsm.h"

class SemQueue {
public:
	void put(KernelSem *pcb);
    SemQueue();
    ~SemQueue();
private:
	class El {
	public:
		KernelSem* sem;
		El* next;
		El(KernelSem *sem){
            this->sem=sem;
            this->next=0;
		}
	};
    El *head;
    friend class System;
    friend class KernelSem;
    friend class Thread;
};

#endif
