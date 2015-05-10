


#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stdincludes.h"

enum MessageType{
    JOINREQ,
    JOINREP,
    PINGREQ,
    PING,
    ACK,
    DUMMYLASTMSGTYPE
};

class Config {
private:
	int Mode;
public:
	Config();
	virtual ~Config();

	void setConfigMode(int mid);
};

#endif 
