#include "kernelsm.h"
#include "queue.h"
#include "SCHEDULE.H"
#include "semaphor.h"
#include "semqueue.h"
#include "system.h"


Semaphore::Semaphore(int init) {
	lock();
	if (init < 0) init = 0;
	myImpl = new KernelSem(init);
	unlock();
}
Semaphore::~Semaphore() {
	lock();
	delete myImpl;
	unlock();
}
int Semaphore::wait(Time maxTimeToWait) {
	int ret;
	lock();
	ret = myImpl->wait(maxTimeToWait);
	unlock();
	return ret;
}
int Semaphore::signal(int n) {
	int ret;
	lock();
	ret = myImpl->signal(n);
	unlock();
	return ret;
}
int Semaphore::val() const {
	int ret;
	lock();
	ret = myImpl->val();
	unlock();
	return ret;
}

KernelSem::KernelSem(int init) {
	value = init;
	timerQueue = new Queue();
	waitQueue = new Queue();
	System::semQueue->put(this);
}
int KernelSem::val() const {
	lock();
	int val = value;
	unlock();
	return val;
}

int KernelSem::wait(Time maxTimeToWait) {
	lock();
	System::running->waitReturn = 1;
	value--;
	if (value < 0) {
		System::running->myState = PCB::BLOCKED;
		if (maxTimeToWait == 0) {
			waitQueue->put(System::running);
		} else {
			System::running->semSleepTime = maxTimeToWait;
			timerQueue->put(System::running);
		}
		dispatch();
	}
	unlock();
	return System::running->waitReturn;
}

int KernelSem::signal(int n) {
	if (n < 0)
		return n;
	if (n == 0) {
		lock();
		value++;
		if (value <= 0) {
			PCB* pcb = waitQueue->get();
			if (pcb == 0)
				pcb = timerQueue->get();
			pcb->myState = PCB::READY;
			Scheduler::put(pcb);
		}
		unlock();
		return 0;
	}
	if (n > 0) {
		int counter = 0;
		lock();
		for (int i=0; i<n; i++) {
			value++;
			if (value <= 0) {
				PCB *pcb = waitQueue->get();
				if (pcb == 0)
					pcb = timerQueue->get();
				pcb->myState = PCB::READY;
				Scheduler::put(pcb);
				counter++;
			}
		}
		unlock();
		return counter;
	}
	//should never happen, if n = NaN for example
	unlock();
	return 0;
}

