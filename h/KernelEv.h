#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "event.h"

typedef unsigned char IVTNo;

class PCB;
class Event;

class KernelEv {
public:
    KernelEv(IVTNo i, Event* e);
    virtual ~KernelEv();

    void signal();
    void wait();
private:
    IVTNo ivtNo;
    Event* myEvent;

    int val;

    PCB* maker;
};

#endif /* KERNELEV_H_ */
