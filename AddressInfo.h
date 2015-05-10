
#ifndef _ADDRESSINFO_H_
#define _ADDRESSINFO_H_

#include "stdincludes.h"
#include "Config.h"


class AddressInfo {
private:
	string SelfAddress;
	short SelfPort;

	string PeerAddress;
	short PeerPort;

	string remoteAddress;
    struct ifaddrs *IfAddress;
    struct sockaddr_in *AddressSelf;
    struct sockaddr_in *AddressPeer;

    bool isSelf;

    int nearNeighbors;
    double joinedTimestamp;
    int nodeIndex;
    
public:
	AddressInfo();
	virtual ~AddressInfo();

	bool operator ==(const AddressInfo &anotherAddress);
	AddressInfo(string address);

	string getSelfAddress();
	string getPeerAddress();

	bool isSelfNode();
	void setNodeMode(int mode);
	void teardown();
};

#endif 