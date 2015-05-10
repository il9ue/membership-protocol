
#include "Broadcasts.h"

Broadcasts::Broadcasts() {
	
}

Broadcasts::~Broadcasts() {
	
}

int Broadcasts::getNodeDescriptor() {
	return this->nodeDescriptor;
}

// TCP
int Broadcasts::setSockets(string addr) {
	struct addrinfo *address;
	struct addrinfo hints;
	int error;

	/* open a TCP socket */
	memset(&hints, 0, sizeof hints);
	hints.ai_family = PF_UNSPEC; /* any supported protocol */
	hints.ai_flags = AI_PASSIVE; /* result for bind() */
	hints.ai_socktype = SOCK_STREAM; /* TCP */
	error = getaddrinfo (addr, "8080", &hints, &address);
	if (error) {
		errx(1, "getaddrinfo failed: %s", gai_strerror(error));
	}

	 memcpy(&nodeAddress, &address, sizeof(addrinfo));
	 memcpy(&remoteIdentity, &hints, sizeof(addrinfo));
}

int Broadcasts::initRemoteDescriptor() {
	int localFD;
	struct addrinfo *addr;
	
	memcpy(&addr, &nodeAddress, sizeof(addrinfo));

	localFD = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (localFD < 0) {
		//error
	}
	this->nodeDescriptor = localFD;
	return localFD;
}

int Broadcasts::bindRemoteNode() {
	int failed = SUCCESS;
	int nodeFD = this->nodeDescriptor;
	struct addrinfo *addr;
	
	memcpy(&addr, &nodeAddress, sizeof(addrinfo));

	if (bind(nodeFD, addr->ai_addr, addr->ai_addrlen) < 0) {
		//error
		failed = FAILURE;
	}

	return failed;
}

int Broadcasts::listenRemoteNodes() {
	int nodeFD = this->nodeDescriptor;
	int failed = SUCCESS;

	if (listen(nodeFD, 5) < 0) {
		// error
		failed = FAILURE;
	}
	return failed;
}

int Broadcasts::acceptRemoteRequest(int fd, struct sockaddr *addr, socklen_t *addrLen) {
	int nodeFD = this->nodeDescriptor;
	int retVal = -1;
	struct addrinfo *addr;
	
	memcpy(&addr, &nodeAddress, sizeof(addrinfo));

	if (accept(nodeFD, addr->ai_addr, addrLen) < 0) {
		// error
	} 
	else {
		retVal = 1;
		this->remoteNodeAccepted = true;
		//memcpy(&nodeAddress, &addr, sizeof(addrinfo));
		addressInfo->setRemoteAddress(addr);
	}
	return retVal;
}


void Broadcasts::sendMessage(int s, char *message, ...) {
    char buf[256];
    int len;

    va_list ap;
    va_start(ap, message);
    len = vsnprintf(buf, sizeof(buf), message, ap);
    va_end(ap);
    send(s, buf, len, 0);
}

void Broadcasts::receiveMessage(int s) {
    char buf[256];
    size_t bytes_read;

    MembershipAgent *agent = new MembershipAgent();

    bytes_read = recv(s, buf, sizeof(buf), 0);
    if (bytes_read != -1) {
        printf("%d bytes read\n", (int)bytes_read);
    }

    agent->queueMessage(s, &buf);
}




