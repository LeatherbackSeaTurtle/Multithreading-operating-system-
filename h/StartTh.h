#ifndef STARTTH_H_
#define STARTTH_H_

#include "thread.h"

extern int userMain(int argc, char* argv[]);

class StartTh: public Thread {
public:
	StartTh(int ac, char** av);
	virtual ~StartTh();

	void run();

	int ret;
private:
	int argc;
	char** argv;
};

#endif /* STARTTH_H_ */
