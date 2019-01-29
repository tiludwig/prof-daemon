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
	HostProtocol* protocol;
private:
	void waitForClient();
	bool isClientConnected();
public:
	TcpPort(HostProtocol* proto);
	virtual ~TcpPort();

	virtual bool initialize();
	virtual Request* waitForRequest();
	virtual bool sendResponse(Serializable& response);
};

#endif /* HOSTPORT_TCPPORT_HPP_ */
