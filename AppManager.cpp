
#include "AppManager.h"


int main(int argc, char *argv[]) {
	//signal(SIGSEGV, handler);
	if ( argc != ARGS_COUNT ) {
		cout<<"Configuration (i.e., *.conf) file File Required"<<endl;
		return FAILURE;
	}

	// Create a new application object
	//AppManager *app = new AppManager(argv[1]);

	// default local mode
	string mode = "DEFAULT";
	AppManager *app = new AppManager(mode);

	// Call the run function

	// When done delete the application object

	return SUCCESS;
}


AppManager::AppManager(string mode) {
	int mid = 0;

	switch (mode) {
		case "DEFAULT":
		{
			configInfo->setConfigMode(mid);
			break;
		}
		case "LAN":
		{
			mid = 1;
			configInfo->setConfigMode(mid);			
			break;
		}
		case "WAN":
		{
			mid = 2;
			configInfo->setConfigMode(mid);
			break;
		}
		case "LOSSY":
		{
			mid = 3;
			configInfo->setConfigMode(mid);
			break;
		}
	}

	initialize();
}

AppManager::AppManager() {
	addressInfo = new AddressInfo();
	configInfo = new Config();
	clusterInfos = new Clusters();
	membershipAgent = new MembershipAgent();
	observers = new Observer();
}

AppManager::~AppManager() {
	delete addressInfo;
	delete configInfo;
	delete clusterInfos;
	delete membershipAgent;
	delete observers;
}


void AppManager::initialize() {
	// automatically register local node address
	if (clusterInfos->getEntrySize() == 0 && addressInfo->isEntryListEmpty()) {
		// register self node
		registerNode(SELF_NODE);
	}
	else {

	}
}

void AppManager::introduceEntry(string address) {
	string tmpNodeStr, reqNodeStr;
	tmpNodeStr = addressInfo->getSelfAddress();

	if (strcmp(entryNode, tmpNodeStr) == 0) {
		return;
	}
	else {
		// ready to add peers.. set new address object into global attr
		size_t peerLen = addressInfo->getPeerAddress().length();
		if (peerLen == 0) {
			addressInfo->putPeerAddress(address);
		} 
		else {
			// we have to clarify that all processes are completed with the address
			return;
		}
		strcpy(reqNodeStr, addressInfo->getPeerAddress());

		clusterInfos->registerRemoteNode(reqNodeStr);
		registerNode(PEER_NODE);
	}
	return;
}

int AppManager::registerNode(int mode) {
	string tmpNodeStr;
	int index, nEntries;
	AddressInfo nodeInfo;
	time_t tNow;
	double currentTime = time(&tNow); 

	switch (mode) {
		case SELF_NODE:
		{
			tmpNodeStr = addressInfo->getSelfAddress();

			nEntries = clusterInfos->getEntrySize();
			nodeInfo = clusterInfos->getCurrentNode();
			if (nodeInfo.getNodeIndex() == 0) {
				clusterInfos->setCurrentNode(nEntries + 1, tmpNodeStr, nEntries, currentTime, true);
			}

			&nodeInfo = clusterInfos->getCurrentNode();
			index = clusterInfos->addNodeEntry(nodeInfo);

			if (index == 0) { // latest
				latestClusterIndex = clusterInfos->getEntrySize();
			}
			else {
				latestClusterIndex = index;
			}
			break;
		}
		case PEER_NODE:
		{
			// peer entry register..
			joinPeerNode();
			break;
		}
	}
}

void AppManager::setNodeConfig(AddressInfo addrInfo) {

}

// make cluster as server mode to wait for the message from remote host(node)..
void AppManager::joinPeerNode() {
	// join for the 1st time..
	MessageType mType = JOINREQ;
	int index, nEntries;
	AddressInfo nodeInfo;
	string peerNodeStr;
	int processFailed;

	time_t tNow;
	double currentTime = time(&tNow); 

	nEntries = clusterInfos->getEntrySize();

	clusterInfos->setCurrentNode(nEntries + 1, tmpNodeStr, nEntries, currentTime, false);
	&nodeInfo = clusterInfos->getCurrentNode();

	if (checkClusterState() <= 0) {
		// cluster is not initialized
		string remoteNode = clusterInfos->fetchCurrentRemoteNode();
		size_t remoteLen = remoteNode.length();
		if (remoteLen > 0) {
			clusterInfos->addNodeEntry(remoteNode);
		}
		strcpy(peerNodeStr, remoteNode);
	}
	else {
		// cluster is multiples..
		int i;
		for (i = 0;i < clusterEntryList.size();i++) {
			if (clusterEntryList.at(i)->isReady()) {
				strcpy(peerNodeStr, clusterEntryList.at(i)->fetchCurrentRemoteNode());
				clusterEntryList->processStarted(true);
				break;
			}
		}
	}

	return;
}

void AppManager::loadObserver(string addr) {
	int initObs, processFailed, queueFD;
	string peerNodeStr;
	string remoteNodeStr;

	this->lastMsgQueueSize = membershipAgent->countMsg();
	int lastMsgSize = this->lastMsgQueueSize;
	initObs = observers->initObserver();

	if (initObs == FAILURE) {
		return;
	}

	queueFD = observers->getEventSet();

	processFailed = clusterInfos->startProcess(peerNodeStr);
	if (processFailed < 0) {
		// do next..
	}
	
	observers->runMonitoringEvents(queueFD);

	while (lastMsgSize == this->lastMsgQueueSize) {
		if (lastMsgSize < membershipAgent->countMsg()) {
			remoteNodeStr = addressInfo->getRemoteAddress();
			break;
		}
	}

	membershipAgent->joinMembership(remoteNodeStr);
}

int AppManager::checkClusterState() {
	int state = 0;
	if (clusterEntryList.size() == 0 || clusterEntryList.empty()) {
		state = -1;
	}
	else {
		state = clusterEntryList.size();
	}

	return state;
}


