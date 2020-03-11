#ifndef EVENT_H_
#define EVENT_H_

#include "IVTEntry.h"
#include "KernelEv.h"


#define PREPAREENTRY(ivtNumb,callOldINT)\
void interrupt intRoutine##ivtNumb(...);\
IVTEntry entry##ivtNumb(ivtNumb,intRoutine##ivtNumb);\
void interrupt intRoutine##ivtNumb(...){\
	if (entry##ivtNumb.eventWaiting != 0)\
		entry##ivtNumb.eventWaiting->signal();\
	if(callOldINT == 1)\
		IVTEntry::oldINT(ivtNumb);}


typedef unsigned char IVTNo;
class KernelEv;

class Event {
public:
    Event (IVTNo ivtNo);
    ~Event ();

    void wait();

protected:
    friend class KernelEv;
    void signal(); // can call KernelEv
private:
    KernelEv* myImpl;
};

#endif /* EVENT_H_ */
