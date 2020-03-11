#include "thread.h"
#include "PCB.h"
#include "Schedule.h"
#include "Timer.h"
#include "Queue.h"

Thread::Thread(StackSize stackSize, Time timeSlice){
    lockInt;
	myPCB = new PCB(stackSize,timeSlice,this);
    unlockInt;
}

Thread::~Thread(){
	waitToComplete();
	lockInt;
	if (myPCB != 0)
		delete myPCB;
	unlockInt;
}

void Thread::start(){
	myPCB->createStack(PCB::wrapper);
	if (myPCB->state == NEW){
		myPCB->state = READY;
        lockInt;
        PCB::usefulPCBs++;
		Scheduler::put(myPCB);
        unlockInt;
    }
}

void Thread::waitToComplete(){
    lockCont;
    if (myPCB != ((PCB*)PCB::running) && myPCB->state != FINISHED && myPCB->state != HELP){
        ((PCB*)PCB::running)->state = BLOCKED;
        myPCB->blockedByMe->put((PCB*)PCB::running);
        unlockCont;
        dispatch();
        return;
    }
    unlockCont;
}

void Thread::sleep(Time timeToSleep){
    ((PCB*)PCB::running)->sleep(timeToSleep);
}

void dispatch (){
    lockInt;
    Timer::context_switch = 1;
    Timer::timer();
    unlockInt;
}
