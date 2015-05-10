

#ifndef _APPMANAGER_H_
#define _APPMANAGER_H_

#include "stdincludes.h"
#include "Config.h"


int nodeCount = 0;


#define ARGS_COUNT 2

#define SELF_NODE 1
#define PEER_NODE 2

/**
 * CLASS NAME: Application
 *
 * DESCRIPTION: Application layer of the distributed system
 */
class AppManager {
private:
	// Address for introduction to the group
	// Coordinator Node

	Config *configInfo;
	AddressInfo *addressInfo;
	Clusters *clusterInfos;
	MembershipAgent *membershipAgent;

	Observer *observers;
	string entryNode;
	int latestClusterIndex;

	int lastMsgQueueSize;
	std::vector<Clusters> clusterEntryList;

public:
	AppManager(char *);
	AppManager(string mode);
	virtual ~AppManager();

	Address getjoinaddr();
	int run();
	void mp1Run();
	void fail();
};

#endif /* _APPLICATION_H__ */
