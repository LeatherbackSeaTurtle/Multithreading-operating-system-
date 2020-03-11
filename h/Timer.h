#ifndef TIMER_H_
#define TIMER_H_

#include <dos.h>

//Zabrana prekida
/*
#define lockInt asm{\
	pushf;\
	cli;\
	}
*/

#define lockInt asm pushf;asm cli;

#define unlockInt asm popf;

//Zabrana promene konteksta
#define lockCont Timer::lockFlag=0;

#define unlockCont Timer::lockFlag=1;\
	if(Timer::context_switch == 1){\
	dispatch();\
	}

typedef void interrupt (*pInterrupt)(...);
extern void tick();

class Timer{
public:
    Timer(){};
	virtual ~Timer(){};

    static void interrupt timer(...);
    static void inic();
    static void restore();

    static volatile int context_switch;
    static volatile int cntr;
    static volatile int lockFlag;
};

#endif /* TIMER_H_ */
