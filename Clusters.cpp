
#include "Clusters.h"

Clusters::Clusters() {
	
}

Clusters::~Clusters() {
	
}

int Clusters::getClusterMode() {
	return this->ClusterMode;
}

void Clusters::setCurrentNode(string address, int nnb, double ts, int id, bool self) {
	AddressInfo currentAddress = new AddressInfo();

	if (self) {
		currentAddress = AddressInfo(id, address, nnb, ts, self);
		this->addressInfo = currentAddress;
	}
	else {
		currentAddress = AddressInfo(id, address, nnb, ts, self);
		//this->addressInfo = currentAddress;
	}
}

void Clusters::setClusterMode(int mode) {
	this->ClusterMode = mode;
}

void Clusters::clearPeerList() {
	NodeEntryList.clear();
}

string Clusters::getSelfAddress() {
	this->LocalCluster = addressInfo->getSelfAddress();
	return this->LocalCluster;
}

int Clusters::sendMessage(string rAddress, string remoteMsg) {

	std::vector<AddressInfo>::iterator myPos;
	int i;

	if (NodeEntryList.empty()) {
		//return FAILURE; // no member node to reach..
	}

	configNode(rAddress);

	// send msg to peerlist memebers..
	for (i = 0;i < NodeEntryList.size();i++) {

	}
}

int Clusters::getEntrySize() {
	return NodeEntryList.size();
}

int Clusters::addNodeEntry(string address) {
	string tmpStr;
	int i, index;
	AddressInfo tmpAddr;

	for (i = 0;i < NodeEntryList.size();i++) {
		strcpy(&tmpStr, &NodeEntryList.at(i).getSelfAddress());
		if (0 == strcmp(tmpStr, address)) {
			index = i;
			break;
		}
		else {
			index = 0;
			continue;	
		}
	}
	tmpAddr = AddressInfo(address);
	NodeEntryList.emplace_back(tmpAddr);
	this->addressInfo = tmpAddr;

	return index;
}

bool Clusters::isOnProcess() {
	return this->onProcess;
}

void Clusters::processStarted() {
	this->onProcess = true;
	this->beforeProcess = false;
}

bool Clusters::isReady() {
	return this->beforeProcess;
}

void Clusters::registerRemoteNode(string address) {
	this->RemoteCluster = address;
}

string Clusters::fetchCurrentRemoteNode() {
	return this->RemoteCluster;
}

AddressInfo *Clusters::getCurrentNode() {
	return this->addressInfo;
}

bool Clusters::isEntryListEmpty() {
	return NodeEntryList.empty() ? true : false;
}

// put address into list
void Clusters::configNode(string rAddress) {
	AddressInfo addrInfo = new AddressInfo();
	string peerStr;
	int i;

	if (0 == strcmp(rAddress, getSelfAddress())) {
		return FAILURE;
	}

	peerStr = addressInfo->getPeerAddress();
	for (i = 0;i < NodeEntryList.size();i++) {
		if (strcmp(NodeEntryList.at(i).getSelfAddress(), peerStr) == 0) {
			// already in list
			break;
		}
		addressInfo = AddressInfo(peerStr);
	}
	NodeEntryList.emplace_back(addressInfo);
}


int Clusters::startProcess(string addr) {
	int isFailed;
	Broadcasts *netAgent = new Broadcasts();

	isFailed = netAgent->setSockets(addr);
	if (isFailed < 0) {
		// error
	}
	
	if (netAgent->initRemoteDescriptor() < 0) {

	}

	if (netAgent->bindRemoteNode() < 0) {

	}

	if (netAgent->listenRemoteNodes() < 0) {

	}

	return isFailed;
}


int Clusters::findConnectionIndex(int fd) {
    int uidx;
    for (uidx = 0; uidx < NUM_CLIENTS; uidx++) {
        
        if (connectClients[uidx].uc_fd == fd) {
            return uidx;
        }
    }
    return -1;
}

/* add a new connection storing the IP address */
int Clusters::addConnection(int fd) {
    int uidx;
    if (fd < 1) return -1;
    if ((uidx = findConnectionIndex(0)) == -1) {
        return -1;
    }

    if (uidx == NUM_CLIENTS) {
        close(fd);
        return -1;
    }

    connectClients[uidx].uc_fd = fd; /* users file descriptor */
    connectClients[uidx].uc_addr = 0; /* user IP address */
    return 0;
}

/* remove a connection and close it's fd */
int Clusters::deleteConnection(int fd) {
    int uidx;
    if (fd < 1) return -1;
    if ((uidx = findConnectionIndex(fd)) == -1) {
        return -1;
    }

    connectClients[uidx].uc_fd = 0;
    connectClients[uidx].uc_addr = NULL;

    return close(fd);
}

