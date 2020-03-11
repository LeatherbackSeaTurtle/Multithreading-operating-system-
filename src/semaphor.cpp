#include "semaphor.h"
#include "kSem.h"
#include "Timer.h"

Semaphore::Semaphore(int init){
    lockInt;
    myImpl = new KernelSem(init, this);
    unlockInt;
}

Semaphore::~Semaphore() {
	if (myImpl != 0){
		lockInt;
		delete myImpl;
		unlockInt;
	}
}

int Semaphore::wait (int toBlock) {
    if(toBlock == 0 && val() <= 0)
    	return -1;
    return myImpl->wait();
}

void Semaphore::signal() { myImpl->signal(); }

int Semaphore::val () const { return myImpl->value(); }
