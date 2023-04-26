#include "idle.h"
#include "pcb.h"


Idle::Idle() : Thread(256, 1) {}

void Idle::start(){
	lock();
	myPCB->myState = PCB::READY;
	myPCB->newStack();
	unlock();
}
void Idle::run() {
	while(1) {};
}
