/*
 * TcpPort.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#ifndef HOSTPORT_TCPPORT_HPP_
#define HOSTPORT_TCPPORT_HPP_

#include "HostPort.hpp"
#include <netinet/in.h>
#include "Protocol/HostProtocol.hpp"



class TcpPort : public HostPort
{
private:
	int serverSocket;
	int clientSocket;
	bool isInResponseState;
	std::unique_ptr<HostProtocol> protocol;
private:
	void waitForClient();
	bool isClientConnected();
public:
	TcpPort(std::unique_ptr<HostProtocol> proto);
	virtual ~TcpPort();

	virtual bool initialize();
	virtual std::unique_ptr<Request> waitForRequest();
	virtual bool sendResponse(Serializable& response);
};

#endif /* HOSTPORT_TCPPORT_HPP_ */
