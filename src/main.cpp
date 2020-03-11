#include "thread.h"
#include "HThread.h"
#include "Timer.h"
#include "PCB.h"
#include <iostream.h>
#include "Schedule.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){

	PCB::startPCB = (volatile PCB*)new PCB(2,0,0);
	PCB::inic = 1;

	HThread* help = new HThread();
	help->start();

	Timer::inic();
	int temp = userMain(argc, argv);
	Timer::restore();

	help->tru = 0;

	delete help;
	delete PCB::startPCB;


	return temp;
}
