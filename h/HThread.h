#ifndef HTHREAD_H_
#define HTHREAD_H_

#include "thread.h"

class HThread: public Thread {
public:
	HThread();
	virtual ~HThread(){}
	void run();

	int tru;
};

#endif /* HTHREAD_H_ */
