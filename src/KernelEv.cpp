#include "KernelEv.h"
#include "Schedule.h"
#include "PCB.h"
#include "event.h"
#include "IVTEntry.h"
#include "thread.h"
#include "Timer.h"

KernelEv::KernelEv(IVTNo i, Event* e): ivtNo(i), myEvent(e), val(1){
    maker = ((PCB*)PCB::running);
    IVTEntry::set(ivtNo, this);
}

KernelEv::~KernelEv(){
    lockCont;
    if (val == 0){
        maker->state = READY;
        Scheduler::put(maker);
    }
    unlockCont;
    if (IVTEntry::IVT[ivtNo]->eventWaiting == this)
    	IVTEntry::IVT[ivtNo]->eventWaiting = 0;
    maker = 0;
    myEvent = 0;
}

void KernelEv::signal(){
    lockCont;
    if (val == 0){
        maker->state = READY;
        val = 1;
        Scheduler::put(maker);
    }
    unlockCont;
}

void KernelEv::wait(){
    lockCont;
    if (( val == 1) && (maker == ((PCB*)PCB::running))){
        maker->state = BLOCKED;
        val = 0;
        unlockCont;
        dispatch();
        return;
    }
    unlockCont;
}
