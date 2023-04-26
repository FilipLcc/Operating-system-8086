#include <dos.h>
#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"
#include "thread.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock();
	this->myPCB = new PCB(this, timeSlice, stackSize);
	unlock();
}
Thread::~Thread(){
	lock();
	waitToComplete();
    delete this->myPCB;
    unlock();
}
void Thread::start(){
	lock();
	this->myPCB->newStack();
	this->myPCB->myState = PCB::READY;
	Scheduler::put(myPCB);
	unlock();
}
void Thread::waitToComplete(){
	lock();
	if(this->myPCB->myState == PCB::DONE || this->myPCB->myState == PCB::NEW) {
		unlock();
		return;
	}
	if(this->myPCB == System::running || this == System::first || this == System::idle) {
		unlock();
		return;
	}
	System::running->myState = PCB::BLOCKED;
	this->myPCB->incompleteQueue->put(System::running);
	dispatch();
	unlock();
}

void Thread::run() {}


ID Thread::getId(){
	ID ret;
    lock()
	ret = this->myPCB->myID;
	unlock();
	return ret;
}
ID Thread::getRunningId(){
	ID ret;
	lock()
	ret = System::running->myID;
	unlock()
	return ret;
}

Thread* Thread::getThreadById(ID id){
	Thread *thread = 0;
	lock();
	Queue::Elem *cur = System::PCBQueue->head;
	while(cur) {
		if (cur->pcb->myID == id) {
			thread = cur->pcb->myThread;
			break;
		}
		cur = cur->next;
	}
	unlock();
	return thread;
}
void dispatch(){
	lock();
	System::dispatch();
	unlock();
}



