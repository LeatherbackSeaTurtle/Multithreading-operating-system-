#include "PCB.h"
#include "thread.h"
#include "Queue.h"
#include <dos.h>
#include "Timer.h"

Queue* PCB::sleepingPCBs = new Queue();;
volatile int PCB::usefulPCBs = 0;
StackSize PCB::MAXstackSize = 65536/4;
volatile PCB* PCB::running = 0;
volatile PCB* PCB::helpPCB = 0;
volatile PCB* PCB::startPCB = 0;
volatile int PCB::inic = 0;

PCB::PCB(StackSize sts, Time ts, Thread* t) : timeSlice(ts), myThread(t), stack(0), finishedProperly(0) {
    if (sts>MAXstackSize)
    	sts = MAXstackSize - 1;
	stackSize = (sts/sizeof(unsigned)); //broj elemenata steka
	sp = 0; ss = 0; bp = 0;
    timeToSleep = 0;
    state = NEW;
    lockInt;
    blockedByMe = new Queue();
    if (PCB::inic == 1){
    	PCB::helpPCB = this;
    	PCB::running = PCB::startPCB;
    	state = HELP;
    	((PCB*)PCB::startPCB)->state = HELP;
    	PCB::inic++;
    }
    unlockInt;
}

PCB::~PCB() {
    lockInt;
	if (stack != 0)
		delete[] stack;
	stack = 0;
    if (finishedProperly == 0){
        if (blockedByMe != 0)
        	blockedByMe->unblockAndRemAll();
        if (state != HELP)
        	PCB::usefulPCBs--;
    }
    if (blockedByMe != 0)
        delete blockedByMe;
    unlockInt;
    myThread = 0;
}


void PCB::createStack(void (*body)()) {
    lockInt;
	unsigned* st1 = new unsigned[stackSize];
    unlockInt;
	st1[stackSize - 1] = 0x200; // setovan I fleg u pocetno PSW-u za nit
#ifndef BCC_BLOCK_IGNORE
			st1[stackSize - 2] = FP_SEG(body);
			st1[stackSize - 3] = FP_OFF(body);
//ostavljamo mesto za osam registara opste namene
			sp = FP_OFF(st1 + stackSize - 12);

			ss = FP_SEG(st1 + stackSize - 12); // svi sacuvani registri pri ulasku u interrupt rutinu
			bp = FP_OFF(st1 + stackSize - 12);
#endif
	stack = st1;
}

void PCB::wrapper(){
    ((PCB*)PCB::running)->myThread->run();
    lockCont;
    if (((PCB*)PCB::running)->blockedByMe != 0)
    	((PCB*)PCB::running)->blockedByMe->unblockAndRemAll();
    if (((PCB*)PCB::running)->state != HELP) {
    	((PCB*)PCB::running)->state = FINISHED;
    	PCB::usefulPCBs--;
    }
    ((PCB*)PCB::running)->finishedProperly = 1;
    unlockCont;
    dispatch();
}

void PCB::sleep(Time tts){
	lockCont;
    state = SLEEPING;
    timeToSleep = tts;
    unlockCont;
    dispatch();
}

void PCB::updateSleepTime(Time tts){
    timeToSleep = tts;
}

Time PCB::sleepTime(){
    return timeToSleep;
}
