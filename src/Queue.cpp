#include "Queue.h"
#include "PCB.h"
#include "Timer.h"
#include "Schedule.h"

Queue::Queue() : first(0), last(0){}

Queue::~Queue(){ deleteAll(); }

void Queue::deleteAll(){
    lockInt;
    for(Elem* temp = first; first != 0; temp = first){
    	first = first->next;
        temp->next = 0;
        temp->p = 0;
        delete temp;
    }
    last = 0;
    unlockInt;
}

void Queue::put(PCB* t){
    lockInt;
    Elem* temp = new Elem(t);
    if(first == 0) first = temp; else last->next = temp;
    last = temp;
    unlockInt;
}

PCB* Queue::get(){
    if(first==0) return 0;
    lockInt;
    PCB* t = first->p;
    Elem* temp = first;
    first = first->next;
    if (first == 0) last = 0;
    temp->next = 0;
    temp->p = 0;
    delete temp;
    unlockInt;
    return t;
}

void Queue::awake(int a){
    lockInt;
    for(Elem* temp = first; temp != 0;){
        if (temp->p->timeToSleep == 0) {
        	temp->p->state = READY;
        	Scheduler::put(temp->p);
        	PCB* rem = temp->p;
        	temp = temp->next;
        	remove(rem);
        	rem = 0;
        }
        else {
        	temp->p->updateSleepTime(temp->p->timeToSleep - a);
        	temp = temp->next;
        }
    }
    unlockInt;
}


void Queue::unblockAndRemAll(){
    lockCont;
    for(Elem* temp = first; temp != 0;){
        temp->p->state = READY;
        Scheduler::put(temp->p);
        Elem* rem = temp;
        temp = temp->next;
        rem->next = 0;
        rem->p = 0;
        lockInt;
        delete rem;
        unlockInt;
    }
    first = last = 0;
    unlockCont;
}

void Queue::remove(PCB* pom){
    lockInt;
    Elem* prev = 0;
    for(Elem* temp = first; temp != 0; temp = temp->next){
        if(pom == temp->p){
            if(prev == 0) {
            	first = first->next;
            	if (first == 0) last = 0;
            	temp->next = 0;
            	temp->p = 0;
            	delete temp;
            	//unlockInt;
            	return;
            }
            if(temp->next == 0)
            	last = prev;
            if (prev != 0 && temp != 0)
            	prev->next = temp->next;
            temp->next = 0;
            temp->p = 0;
            delete temp;
            unlockInt;
            return;
        }
        prev = temp;
    }
    unlockInt;
}
