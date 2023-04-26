
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "idle.h"
#include "pcb.h"
#include "thread.h"
#include "IVTEntry.h"


#define lock() asm {pushf; cli;}
#define unlock() asm {popf;}

class Queue;
class SemQueue;

class System {
public:
	static Queue* PCBQueue;
	static SemQueue *semQueue;
	static Thread* first;
	static Idle* idle;
	static PCB* running;
	static int dispatched;

	static void dispatch();
	static void RiseAndShine();
	static void turnOn();
	static void turnOff();
	static void wrapper();
	static void interrupt timer(...);
	static void interrupt (*oldRoutine)(...);

	friend class IVTEntry;
	friend class PCB;
	friend class Thread;
};


#endif
