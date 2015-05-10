

#ifndef _BROADCASTS_H_
#define _BROADCASTS_H_

#include "stdincludes.h"
#include "Config.h"


class Broadcasts {
private:
	struct addrinfo *nodeAddress;
	struct addrinfo remoteIdentity;
	struct sockaddr_in clusterServer;
	struct sockaddr_in clusterClient;

    int nodeDescriptor;

    AddressInfo *addressInfo;

public:
	Broadcasts();
	virtual ~Broadcasts();

	Broadcasts(string address);
};

#endif 