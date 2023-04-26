#include <dos.h>
#include "pcb.h"


ID PCB::globalID = 0;


PCB::PCB(Thread* myThread, Time timeSlice, StackSize stackSize) {
	lock();
	this->myThread = myThread;
	this->myTimeSlice = timeSlice;
	this->myID = globalID++;
	if(stackSize>65535) stackSize = 65535;
	this->myStackSize = stackSize/sizeof(unsigned);
	this->myState = PCB::NEW;
	this->SP=0;
	this->passedTime = 0;
	this->semSleepTime = 0;
	this->waitReturn = 0;
	this->incompleteQueue = new Queue();
	System::PCBQueue->put(this);
	unlock();
}
PCB::~PCB(){
	lock();
	delete SP;
	delete incompleteQueue;
	unlock();
}

void PCB::newStack(){
	lock();
	this->SP = new unsigned[this->myStackSize];
	this->SP[this->myStackSize-1] = 0x200;
	this->SP[this->myStackSize-2] = FP_SEG(&(System::wrapper));
	this->SP[this->myStackSize-3] = FP_OFF(&(System::wrapper));
	this->sp=FP_OFF(this->SP+this->myStackSize-12);
	this->ss=FP_SEG(this->SP+this->myStackSize-12);
	unlock();
}

