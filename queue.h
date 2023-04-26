#ifndef QUEUE_H_
#define QUEUE_H_

#include "pcb.h"
#include "system.h"
#include "thread.h"

class Queue {
public:
    Queue();
    ~Queue();
	void put(PCB *pcb);
    PCB* get();
private:
	class Elem {
	public:
		PCB* pcb;
		Elem* next;
		Elem(PCB* pcb){
            this->pcb=pcb;
            this->next=0;
		}
	};
    Elem* head;
    friend class System;
    friend class PCB;
    friend class Thread;
    friend class KernelSem;
};


#endif
