
#include "Observer.h"

Observer::Observer() {
	nodeNet = new Broadcasts();
}

Observer::~Observer() {
	delete nodeNet;
}

int Observer::initObserver() {
	int localFD = nodeNet->getNodeDescriptor();

	this->obsQueue = kqueue();
	EV_SET(&this->nodeEventSets, localFD,
			EVFILT_READ, EV_ADD, 0, 0, NULL);

	if (kevent(this->obsQueue, &this->nodeEventSets, 1, NULL, 0, NULL) < 0) {
		// error
		return FAILURE;
	}

	return SUCCESS;
}

int Observer::getEventSet() {
	return this->nodeEventSets;
}

// fix later..
void Observer::runMonitoringEvents(int kq) {
	struct kevent evSet;
	struct kevent evList[32];
	int nEvents, i;
	struct sockaddr_storage addr;
	socklen_t socklen = sizeof(addr);
	int fd;

	int sockFD = nodeNet->getNodeDescriptor();
	while(ON_CONNECT) {
		nEvents = kevent(kq, NULL, 0, evList, 32, NULL);
		if (nEvents < 1) {

		}
		for (i = 0; i < nEvents; i++) {
			if (evList[i].flags & EV_EOF) {
				//printf("disconnect\n");
				fd = evList[i].ident;
				EV_SET(&evSet, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);

				if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {

				}
				nodeNet->deleteConnection(fd);
			}
			else if (evList[i].ident == sockFD) {
				//fd = accept(evList[i].ident, (struct sockaddr *)&addr, &socklen);
				fd = nodeNet->acceptRemoteRequest(evList[i].ident, (struct sockaddr *)&addr, &socklen);

				if (fd == -1) {
				  //err(1, "accept");
				}
				
				if (nodeNet->addConnection(fd) == 0) {
				  EV_SET(&evSet, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
				  
				  if (kevent(kq, &evSet, 1, NULL, 0, NULL) == -1) {
				      err(1, "kevent");
				  }
				  //send_msg(fd, "welcome!\n");
				} else {
				  //printf("connection refused\n");
				  close(fd);
				}
			}
			else if (evList[i].flags & EVFILT_READ) {
				//recv_msg(evList[i].ident);
			}
		}
	}
}