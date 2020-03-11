#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include <dos.h>
#include "event.h"

typedef unsigned char IVTNo;
typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry {
public:
    IVTEntry(IVTNo n, pInterrupt newINT);
    virtual ~IVTEntry();

    KernelEv* eventWaiting;

    static IVTEntry* get(IVTNo i);
    static void set(IVTNo i, KernelEv* e);
    static void oldINT(IVTNo i);
protected:
    friend class KernelEv;
private:
    static IVTEntry* IVT[256];

    IVTNo ivtNum;
    pInterrupt oldRoutine;
};

#endif /* IVTENTRY_H_ */
