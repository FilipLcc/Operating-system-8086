#include "event.h"
#include "IVTEntry.h"
#include "kernelev.h"
#include "system.h"

Event::Event (IVTNo ivtNo) {
	lock();
		myImpl = new KernelEv(ivtNo);
	unlock();
}
Event::~Event () {
	lock();
		delete myImpl;
	unlock();
}
void Event::wait () {
	lock();
		myImpl->wait();
	unlock();
}
void Event::signal() {
	lock();
		myImpl->signal();
	unlock();
}
