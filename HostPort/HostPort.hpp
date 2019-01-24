

#ifndef HOSTPORT_HPP_
#define HOSTPORT_HPP_

#include "Serializable.hpp"

class HostPort
{
public:
	virtual ~HostPort() {}

	virtual bool initialize() = 0;
	virtual bool waitForRequest() = 0;
	virtual bool sendResponse(Serializable& response) = 0;
};

#endif
