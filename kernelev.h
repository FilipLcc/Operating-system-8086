#ifndef KERNELEV_H_
#define KERNELEV_H_

class KernelSem;
class Thread;
class PCB;


typedef unsigned char IVTNo;


class KernelEv {
public:
	KernelEv (IVTNo ivtNo);
	~KernelEv ();
	void wait ();
	void signal();
private:
	IVTNo ivtNo;
	KernelSem* mySem;
	Thread* owner;
};


#endif
