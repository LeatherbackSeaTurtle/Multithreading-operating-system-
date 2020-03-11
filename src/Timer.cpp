#include "Timer.h"
#include <dos.h>
#include "PCB.h"
#include "Queue.h"
#include "Schedule.h"

volatile int Timer::context_switch = 0;
volatile int Timer::cntr = defaultTimeSlice;
volatile int Timer::lockFlag = 1;

unsigned tsp;
unsigned tss;
unsigned tbp;

pInterrupt oldTimerRoutine;

void interrupt Timer::timer(...){

    if(Timer::context_switch == 0 && Timer::cntr > 0)
    	Timer::cntr--;

    if(Timer::context_switch == 0) {
        oldTimerRoutine();
        if(PCB::sleepingPCBs != 0)
        	PCB::sleepingPCBs->awake(1);
        tick();
    }


    if ( (Timer::cntr == 0 && ((PCB*)PCB::running)->timeSlice != 0) || Timer::context_switch == 1) {
        if(Timer::lockFlag == 1){
            asm {
                mov tsp, sp
                mov tss, ss
                mov tbp, bp
            }

            ((PCB*)PCB::running)->sp = tsp;
            ((PCB*)PCB::running)->ss = tss;
            ((PCB*)PCB::running)->bp = tbp;

            if(((PCB*)PCB::running)->state == SLEEPING)
            	PCB::sleepingPCBs->put(((PCB*)PCB::running));

            if (((PCB*)PCB::running)->state == READY && PCB::running != PCB::helpPCB &&
            		PCB::running != PCB::startPCB)
            	Scheduler::put(((PCB*)PCB::running));

            PCB::running = (volatile PCB*)Scheduler::get();

            if (PCB::running == 0){
            	if (PCB::usefulPCBs != 0)
            		PCB::running = PCB::helpPCB;
            	else
            		PCB::running = PCB::startPCB;
            }


            tsp = ((PCB*)PCB::running)->sp;
            tss = ((PCB*)PCB::running)->ss;
            tbp = ((PCB*)PCB::running)->bp;

            Timer::cntr = ((PCB*)PCB::running)->timeSlice;

            asm{
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
            }

        }

        Timer::context_switch = 0;
    }
}

void Timer::inic(){
#ifndef BCC_BLOCK_IGNORE
    oldTimerRoutine = getvect(0x8);
    setvect(0x8, Timer::timer);
    setvect(0x60, oldTimerRoutine);
#endif
}

void Timer::restore(){
#ifndef BCC_BLOCK_IGNORE
    setvect(0x8, oldTimerRoutine);
#endif
}
