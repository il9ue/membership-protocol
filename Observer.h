


#ifndef _OBSERVER_H_
#define _OBSERVER_H_

#include "stdincludes.h"
#include "Config.h"
#include <sys/event.h>

#define ON_CONNECT 	1

class Observer {
private:
	Broadcasts *nodeNet;
	int obsQueue;
	struct kevent nodeEventSets;

	int nodeDescriptor;
	bool remoteNodeAccepted;

public:
	Observer();
	virtual ~Observer();

	Observer(string address);
};

#endif 