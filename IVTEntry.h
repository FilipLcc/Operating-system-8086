#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "event.h"
#include "kernelev.h"

typedef void interrupt (*InterruptRoutine)(...);

class IVTEntry {
public:
	IVTEntry(int,InterruptRoutine);
   ~IVTEntry();
	void signal();
	void oldRoutine();
private:
	KernelEv* myEvent;
	InterruptRoutine oldIR;
	unsigned ivtNo;

	static IVTEntry* IVTable[];

	friend class KernelEv;
};


#endif
