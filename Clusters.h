

#ifndef _CLUSTERS_H_
#define _CLUSTERS_H_

#include "stdincludes.h"
#include "Config.h"

#define NUM_CLIENTS		10


struct connClient {
    int uc_fd;
    char *uc_addr;
} connectClients[NUM_CLIENTS];



class Clusters {
private:
	AddressInfo *addressInfo;

	string LocalCluster;
	string RemoteCluster;
	int ClusterMode; // server(1), client(2)

	Broadcasts *netAgent;

	bool onProcess;
	bool beforeProcess;
	std::vector<AddressInfo> NodeEntryList; // limited peers in list
public:
	Clusters();
	virtual ~Clusters();

	bool operator ==(const Clusters &anotherCluster);
	Clusters(string address);

	int sendMessage(string rAddress, string remoteMsg);
	string getSelfAddress();
	void setClusterMode(int mode);
	int getClusterMode();

	void clearPeerList();
};

#endif 