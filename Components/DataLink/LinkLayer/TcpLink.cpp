/*
 * TcpLink.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "TcpLink.hpp"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

TcpLink::TcpLink()
{
	isConnected = false;
	clientSocket = -1;
}

TcpLink::~TcpLink()
{
}

void TcpLink::setProtocol(std::unique_ptr<LinkProtocol> protocol)
{
	linkProtocol = std::move(protocol);
}

bool TcpLink::initialize()
{
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0)
	{
		return false;
	}

	return true;
}

bool TcpLink::open(const char* ip, uint16_t port)
{
	if (isConnected)
	{
		return true;
	}

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port);
	serverAddress.sin_addr.s_addr = inet_addr(ip);

	if (connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		return false;
	}

	isConnected = true;
	return true;
}

void TcpLink::sendPacket(HostPacket& packet)
{
	auto packetStream = packet.serialize();
	auto dataToSend = packetStream->getBuffer();

	auto totalAmountToSend = packet.getPacketSize();
	auto amountToSend = totalAmountToSend;

	while (amountToSend > 0)
	{
		auto writeResult = write(clientSocket, dataToSend, amountToSend);

		if (writeResult == -1)
		{
			throw "TcpLink: Failed during sending a packet.";
		}

		amountToSend -= writeResult;
		dataToSend += (totalAmountToSend - amountToSend);
	}
}

std::unique_ptr<HostPacket> TcpLink::waitForPacket()
{
	linkProtocol->resetReceiver();

	char value;
	while(!linkProtocol->isPacketComplete())
	{

		auto readResult = read(clientSocket, &value, 1);
		if(readResult == -1)
		{
			linkProtocol->resetReceiver();
		}
		else if(readResult == 0)
		{
			// server disconnected during read ...
			close(clientSocket);
			clientSocket = -1;

			// reconnect

			linkProtocol->resetReceiver();
		}
		else
		{
			linkProtocol->processData(value);
		}
	}

	// packet was received
	auto packet = linkProtocol->getPacket();
	return packet;
}
