/*
 * TcpPort.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#ifndef HOSTPORT_TCPPORT_HPP_
#define HOSTPORT_TCPPORT_HPP_

#include "HostPort.hpp"

class TcpPort : public HostPort
{
private:
	bool isInResponseState;
public:
	TcpPort();
	virtual ~TcpPort();

	virtual bool initialize();
	virtual std::unique_ptr<Request> waitForRequest();
	virtual bool sendResponse(Serializable& response);
};

#endif /* HOSTPORT_TCPPORT_HPP_ */
