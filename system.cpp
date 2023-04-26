#include <dos.h>
#include "queue.h"
#include "SCHEDULE.H"
#include "semqueue.h"
#include "system.h"
#include "kernelsm.h"



PCB* System::running = 0;
Thread* System::first = 0;
Idle* System::idle = 0;
Queue* System::PCBQueue = new Queue();
SemQueue* System::semQueue = new SemQueue();
int System::dispatched = 0;
void interrupt (*System::oldRoutine)(...) = 0;


void System::turnOn() {
	lock();
			System::oldRoutine = getvect(0x08);
			setvect(0x08, System::timer);

			System::first = new Thread();
		    System::first->myPCB->myState = PCB::READY;
		    System::running = System::first->myPCB;

		    System::idle = new Idle();
		    System::idle->start();
		unlock();
}

void System::turnOff() {
		lock();
	    setvect(0x08, System::oldRoutine);
	    unlock();
	    delete System::first;
	    delete System::idle;
	    delete System::PCBQueue;
	    delete System::semQueue;
}

void System::dispatch() {
lock();
	dispatched = 1;
	timer();
	dispatched = 0;
unlock();
}

void System::RiseAndShine() {
	for (SemQueue::El *tek = semQueue->head; tek; tek=tek->next) {
		 Queue::Elem *cur = tek->sem->timerQueue->head;
				Queue::Elem *prev = 0, *del = 0;
				while (cur) {
					cur->pcb->semSleepTime--;
					if (cur->pcb->semSleepTime > 0) {
						prev = cur;
						cur = cur->next;
					}
					else {
						cur->pcb->myState = PCB::READY;
						cur->pcb->waitReturn = 0;
						Scheduler::put(cur->pcb);
						del = cur;
						cur = cur->next;
						if (!prev)
							tek->sem->timerQueue->head = cur;
						else
							prev->next = cur;
						delete del;
						tek->sem->value++;
					}
				}
	}
}

void interrupt System::timer(...) {
	if(!dispatched){
		tick();
		(*oldRoutine)();

		System::RiseAndShine();

		if(running->myTimeSlice == 0) return;
		else running->passedTime++;
		if(running->passedTime < running->myTimeSlice) return;

	}

	dispatched=0;

	if(running->myState == PCB::READY && running!=idle->myPCB)
	   Scheduler::put(running);

	running->sp = _SP;
	running->ss = _SS;

	running = Scheduler::get();
	if (running == 0)
		running = idle->myPCB;

	_SP = running->sp;
	_SS = running->ss;

	running->passedTime = 0;
}
void System::wrapper() {
	running->myThread->run();
	lock();
	running->myState = PCB::DONE;
	PCB* cur = running->incompleteQueue->get();
    while (cur) {
    	cur->myState = PCB::READY;
    	Scheduler::put(cur);
    	cur = running->incompleteQueue->get();
    }

    dispatch();
}



