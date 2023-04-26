#include "pcb.h"
#include "queue.h"
#include "SCHEDULE.H"
#include "system.h"


Queue::Queue() {
	lock();
	head = 0;
	unlock();
}
Queue::~Queue() {
	lock();
	Elem* old;
	while(head){
	old=head;
	head=head->next;
	delete old;
	}
	unlock();
}

PCB* Queue::get() {
	lock();
	if(head == 0) {
		unlock();
		return 0;
	}
	Elem* cur = head;
	head = head->next;
	PCB* pcb=cur->pcb;
	delete cur;
	unlock();
	return pcb;
}
void Queue::put(PCB* pcb){
	Elem* novi = new Elem(pcb);
	lock();
	if (head == 0)
		head = novi;
	else {
		Elem* cur = head;
		while (cur->next) cur = cur->next;
	    cur->next = novi;
	}
	unlock();
}
