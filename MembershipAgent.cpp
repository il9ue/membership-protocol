
#include "MembershipAgent.h"

MembershipAgent::MembershipAgent() {

}

MembershipAgent::~MembershipAgent() {

}

int MembershipAgent::joinMembership(string address) {

	string localStr, remoteStr;
	string lastMsg;
	int i;

	localStr = addrInfo.getSelfAddress();
	remoteStr = addrInfo.getPeerAddress();

	lastMsg = messageEntryList.pop_back();

	switch (lastMsg) {
		case "JOINREQ":
		{
			
			break;
		}
		case "JOINREP":
		{
			break;
		}
	}
}

void MembershipAgent::queueMessage(int sid, const char *msg) {
	string message;

	this->entryDescr = sid;
	message = msg;

	messageEntryList.emplace_back(message);
}

int MembershipAgent::countMsg() {
	return messageEntryList.size();
}