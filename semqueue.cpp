#include "kernelsm.h"
#include "SCHEDULE.H"
#include "semqueue.h"
#include "system.h"

SemQueue::SemQueue() {
	lock();
	head = 0;
	unlock();
}
SemQueue::~SemQueue() {
	lock();
	El* old;
	while(head){
		old = head;
		head = head->next;
		delete old;
	}
	unlock();
}

void SemQueue::put(KernelSem* sem){
	lock();
	El* novi = new El(sem);
	if (head == 0)
		head = novi;
	else{
		El* cur;
		for(cur = head; cur->next; cur = cur->next);
		cur->next=novi;
	}
	unlock();
}




