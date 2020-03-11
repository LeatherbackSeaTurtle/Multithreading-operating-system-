#include "HThread.h"
#include "thread.h"
#include "PCB.h"

HThread::HThread() : Thread(defaultStackSize, 1), tru(1) {}

void HThread::run(){
	while(tru);
}
