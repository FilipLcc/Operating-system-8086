#include "IVTEntry.h"
#include "kernelev.h"
#include "kernelsm.h"
#include "SCHEDULE.H"
#include "system.h"
#include "thread.h"

KernelEv::KernelEv(IVTNo ivtNo) {
	lock();
	this->ivtNo=ivtNo;
	this->owner=System::running->myThread;
	this->mySem = new KernelSem(0);
	IVTEntry::IVTable[ivtNo]->myEvent = this;
	unlock();
}
KernelEv::~KernelEv () {
	lock();
	IVTEntry::IVTable[ivtNo] = 0;
	delete mySem;
	unlock();
}
void KernelEv::wait () {
	lock();
	if (owner != System::running->myThread) {
		unlock();
		return;
	}
	mySem->wait(0);
	unlock();
}

void KernelEv::signal() {
	lock();
	if (mySem->val()==1) {
		unlock();
		return;
	}
	mySem->signal();
	unlock();
}

