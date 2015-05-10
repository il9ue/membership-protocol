

#ifndef _MEMBERSHIPAGENT_H_
#define _MEMBERSHIPAGENT_H_

#include "stdincludes.h"
#include "Config.h"


class MembershipAgent {
private:

	int entryDescr;
	AddressInfo * addrInfo;

	std::vector<string> messageEntryList;
    
public:
	MembershipAgent();
	virtual ~MembershipAgent();

	MembershipAgent(string address);
};

#endif 