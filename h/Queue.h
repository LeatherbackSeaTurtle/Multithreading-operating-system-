#ifndef QUEUE_H_
#define QUEUE_H_

#include "Timer.h"

class PCB;

class Queue{
public:
    Queue();
    virtual ~Queue();

    void put(PCB* t);
    PCB* get();

    void awake(int a);

    void unblockAndRemAll();
    void remove(PCB* pom);
    void deleteAll();

private:
    struct Elem {
        PCB* p;
        Elem* next;

        Elem(PCB* t){
            p = t;
            next = 0;
        }
    };

    Elem* first, *last;
};

#endif /* QUEUE_H_ */
