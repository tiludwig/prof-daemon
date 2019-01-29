#ifndef HOSTPROTOCOL_HPP_
#define HOSTPROTOCOL_HPP_

#include "../Requests/Request.hpp"

class HostProtocol
{
public:
	virtual ~HostProtocol()
	{
	}
	virtual bool isPacketComplete() = 0;
	virtual void appendData(unsigned char data) = 0;
	virtual Request* getPacket() = 0;
	virtual void resetReceiver() = 0;
};

#endif
