#ifndef KSEM_H_
#define KSEM_H_

class Queue;
class Semaphore;

class KernelSem{
public:
    KernelSem(int init, Semaphore* s);
	virtual ~KernelSem();

    virtual int wait();
	virtual void signal();

	int value();
private:
    int val;
    Queue* blocked;
    Semaphore* mySem;
};

#endif /* KSEM_H_ */
