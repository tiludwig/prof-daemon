/*
 * TcpLink.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_TCPLINK_HPP_
#define DATALINK_TCPLINK_HPP_

#include "DataLink.hpp"
#include "../ProtocolLayer/LinkProtocol.hpp"
#include <netinet/in.h>

class TcpLink: public DataLink
{
private:
	bool isConnected;
	int clientSocket;
	sockaddr_in serverAddress;
	std::unique_ptr<LinkProtocol> linkProtocol;
private:
	bool connectToServer();
	bool disconnectFromServer();
public:
	TcpLink();
	virtual ~TcpLink();

	void setProtocol(std::unique_ptr<LinkProtocol> protocol);
	virtual bool initialize();
	virtual bool open(const char* ip, uint16_t port);

	virtual void sendPacket(HostPacket& packet);
	virtual std::unique_ptr<HostPacket> waitForPacket();
};

#endif /* DATALINK_TCPLINK_HPP_ */
