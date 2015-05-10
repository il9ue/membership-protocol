
#include "AddressInfo.h"


AddressInfo::AddressInfo() {

}

AddressInfo::~AddressInfo() {

}

// Parse string address to socket ip and port..
AddressInfo::AddressInfo(string address) {
	/*
	size_t pos = address.find(".");
	int id = stoi(address.substr(0, pos));
	short port = (short)stoi(address.substr(pos + 1, address.size()-pos-1));
	memcpy(&addr[0], &id, sizeof(int));
	memcpy(&addr[4], &port, sizeof(short));
	*/
	initConfig(address);
	return this;
}

AddressInfo::AddressInfo(int id, string addr, int nnb, double ts) {
	this->nodeIndex = id;
	this->nearNeighbors = nnb;
	this->joinedTimestamp = ts;
	this->SelfAddress = addr;
}

AddressInfo::AddressInfo(int id, string addr, int nnb, double ts, bool self) {
	this->nodeIndex = id;
	this->nearNeighbors = nnb;
	this->joinedTimestamp = ts;

	if (self) {
		this->SelfAddress = addr;
	}
	else {
		this->PeerAddress = addr;
	}
}

void AddressInfo::initConfig(string address) {
	int mode;
	if (this->SelfAddress == address) {
		mode = 1;
		setNodeMode(mode);
		this->SelfAddress = address;
	}
	else {
		mode = 2;
		setNodeMode(mode);
		this->PeerAddress = address;
	}
} 

void AddressInfo::setSelfPort(short sPort) {
	this->SelfPort = sPort;
}

void AddressInfo::setPeerPort(short pPort) {
	this->OtherPort = pPort;
}

bool AddressInfo::operator ==(const AddressInfo& anotherAddress) {
	bool compared = false;

	if (strcmp(this->SelfAddress, anotherAddress.SelfAddress) == 0) {
		compared = true;
	}

	return compared;
}

void AddressInfo::makeSelfAddress() {
	string selfAddr = "127.0.0.1";
   struct ifaddrs *ifAddr;
   char *addressChs;

   getifaddrs(IfAddress);

   for (ifAddr = IfAddress; ifAddr; ifAddr = ifAddr->ifa_next) {
     if (ifAddr->ifa_addr->sa_family == AF_INET) {
         AddressSelf = (struct sockaddr_in *) ifAddr->ifa_addr;
         addressChs = inet_ntoa(AddressSelf->sin_addr);

         if (strcmp(selfAddr, addressChs) == 0) {
         	continue;
         }
         else {
        		selfAddr = addressChs;
         }
     }
    }

    freeifaddrs(IfAddress);
    this->SelfAddress = selfAddr;
}

string AddressInfo::getRemoteAddress() {
	return this->remoteAddress;
}

string AddressInfo::getSelfAddress() {
	if (this->SelfAddress) {
		return this->SelfAddress;
	}
	else {
		makeSelfAddress();
		return this->SelfAddress;
	}
}

void AddressInfo::setRemoteAddress(struct sockaddr) {
	struct sockaddr nodeAddress;

	memcpy(&nodeAddress, &sockaddr, sizeof(addrinfo));
	// parse ip address to string and save
}

void AddressInfo::setNeighbors(int nnb) {

}

void AddressInfo::setJoinTimestamp(double ts) {

}

void AddressInfo::setNodeIndex(int index) {

}

int AddressInfo::getNeighbors() {
	return this->nearNeighbors;
}

double AddressInfo::getJoinTimestamp() {
	return this->joinedTimestamp;
}

int AddressInfo::getNodeIndex() {
	this->nodeIndex;
}

string AddressInfo::getPeerAddress() {
	return this->PeerAddress;
}

void AddressInfo::putPeerAddress(string addr) {
	this->PeerAddress = addr;
}

bool AddressInfo::isSelfNode() {
	return this->isSelf;
}

void AddressInfo::setNodeMode(int mode) {
	switch (mode) {
		case 1:	// self, client
			this->isSelf = true;
			break;
		case 2:	// remote, server
			this->isSelf = false;
			break;
	}
}



