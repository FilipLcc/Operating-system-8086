#include <dos.h>
#include "IVTEntry.h"
#include "system.h"
IVTEntry* IVTEntry::IVTable[256] = { 0 };

IVTEntry::IVTEntry(int ivtNo, InterruptRoutine newIR) {
    myEvent = 0;
	this->ivtNo=ivtNo;
	IVTEntry::IVTable[ivtNo] = this;
	lock();
		oldIR = getvect(ivtNo);
		setvect(ivtNo, newIR);
	unlock();
}
IVTEntry::~IVTEntry() {
	lock();
	setvect(ivtNo, oldIR);
	unlock();
}
void IVTEntry::signal() {
       myEvent->signal();
}
void IVTEntry::oldRoutine() {
	oldIR();
}
