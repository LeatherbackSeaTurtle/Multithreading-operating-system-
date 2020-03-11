#include "kSem.h"
#include "semaphor.h"
#include "Queue.h"
#include "PCB.h"
#include "thread.h"
#include "Schedule.h"
#include "Timer.h"

KernelSem::KernelSem(int init, Semaphore* s) : val(init), mySem(s){
	lockInt;
    blocked = new Queue();
    unlockInt;
}

KernelSem::~KernelSem() {
    if (blocked != 0){
        blocked->unblockAndRemAll();
        lockInt;
        delete blocked;
        unlockInt;
    }
    mySem = 0;
}

int KernelSem::wait() {
    lockCont;
    if (--val < 0) {
        ((PCB*)PCB::running)->state = BLOCKED;
        blocked->put(((PCB*)PCB::running));
        unlockCont;
        dispatch();
        return 1;
    }
    unlockCont;
    return 0;
}

void KernelSem::signal() {
    lockCont;
    if (val++ < 0) {
        PCB* temp = blocked->get();
        temp->state = READY;
        Scheduler::put(temp);
    }
    unlockCont;
}

int KernelSem::value() { return val; }
