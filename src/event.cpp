#include "event.h"
#include "KernelEv.h"
#include "Timer.h"

Event::Event (IVTNo ivtNo){
    lockInt;
    myImpl = new KernelEv(ivtNo, this);
    unlockInt;
}

Event::~Event (){
	if (myImpl != 0){
		lockInt;
		delete myImpl;
		unlockInt;
	}
}

void Event::wait(){
    myImpl->wait();
}

void Event::signal(){
    myImpl->signal();
}
