#ifndef PCB_H_
#define PCB_H_

#include "thread.h"

class Thread;
class HThread;
class Queue;
class Timer;
class KernelEv;
class KernelSem;

enum State {NEW, READY, BLOCKED, FINISHED, SLEEPING, HELP};

class PCB {
public:

    PCB(StackSize sts, Time ts, Thread* t);
    virtual ~PCB();

    void createStack(void (*)());
    static void wrapper();

    void sleep(Time tts);
    void updateSleepTime(Time tts);
    Time sleepTime();

    static volatile PCB* helpPCB;
    static volatile PCB* startPCB;

    static volatile int inic;

protected:
    friend class Thread;
    friend class HThread;
    friend class Queue;
    friend class Timer;
    friend class KernelEv;
    friend class KernelSem;

private:
	volatile unsigned bp;
	volatile unsigned sp;
	volatile unsigned ss;
    unsigned* stack;

    StackSize stackSize;
    static StackSize MAXstackSize;
    Time timeSlice;
    Time timeToSleep;
    Thread* myThread;

    State state;
    volatile int finishedProperly;

    static volatile int usefulPCBs;

    Queue* blockedByMe;
    static Queue* sleepingPCBs;
    static volatile PCB* running;
};

#endif /* PCB_H_ */
