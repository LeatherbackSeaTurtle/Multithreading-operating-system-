#include "IVTEntry.h"
#include <dos.h>
#include "Timer.h"

IVTEntry* IVTEntry::IVT[256];

IVTEntry::IVTEntry(IVTNo n, pInterrupt newINT): ivtNum(n), eventWaiting(0) {
	lockInt;
    IVTEntry::IVT[ivtNum] = this;
#ifndef BCC_BLOCK_IGNORE
    oldRoutine = getvect(ivtNum);
    setvect(ivtNum, newINT);
#endif
    unlockInt;
}

IVTEntry::~IVTEntry(){
	lockInt;
#ifndef BCC_BLOCK_IGNORE
		setvect(ivtNum, oldRoutine);
#endif
    IVTEntry::IVT[ivtNum] = 0;
    if (eventWaiting != 0)
    	delete eventWaiting;
    unlockInt;
}

IVTEntry* IVTEntry::get(IVTNo i){
    return IVTEntry::IVT[i];
}

void IVTEntry::set(IVTNo i, KernelEv* e){
    IVTEntry::IVT[i]->eventWaiting = e;
}

void IVTEntry::oldINT(IVTNo i){
    IVTEntry::IVT[i]->oldRoutine();
}
