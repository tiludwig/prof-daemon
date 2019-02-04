#ifndef REQUESTHANDLER_HPP_
#define REQUESTHANDLER_HPP_

#include "../DataLink/HostPacket/HostPacket.hpp"

class BusComponent
{
public:
	virtual ~BusComponent() {}
	virtual void accept(HostPacket* request) = 0;
};

#endif
