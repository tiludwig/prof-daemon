

#ifndef HOSTPORT_HPP_
#define HOSTPORT_HPP_

#include "Serializable.hpp"
#include "Requests/Request.hpp"
#include <memory>

struct request_t
{
	unsigned int type;
	unsigned int length;
	unsigned char* data;
	unsigned int checksum;
};

class HostPort
{
public:
	virtual ~HostPort() {}

	virtual bool initialize() = 0;
	virtual std::unique_ptr<Request> waitForRequest() = 0;
	virtual bool sendResponse(Serializable& response) = 0;
};

#endif
