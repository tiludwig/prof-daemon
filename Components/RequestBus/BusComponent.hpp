#ifndef REQUESTHANDLER_HPP_
#define REQUESTHANDLER_HPP_

#include "../HostPort/Requests/Request.hpp"

class BusComponent
{
public:
	virtual ~BusComponent() {}
	virtual void acceptRequest(Request* request) = 0;
};

#endif
