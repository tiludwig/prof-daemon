#ifndef REQUESTHANDLER_HPP_
#define REQUESTHANDLER_HPP_

#include "../HostPort/Requests/Request.hpp"

class BusComponent
{
public:
	virtual ~BusComponent() {}
	virtual void acceptRequest(std::unique_ptr<Request> request) = 0;
};

#endif
