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
	serverSocket = -1;
}

TcpLink::~TcpLink()
{
	if (clientSocket >= 0)
		close(clientSocket);
	if (serverSocket >= 0)
		close(serverSocket);
}

void TcpLink::setProtocol(std::unique_ptr<LinkProtocol> protocol)
{
	linkProtocol = std::move(protocol);
}

bool TcpLink::initialize()
{
	// Creating socket file descriptor
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		return false;
	}

	const int optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, (void*) &optVal, optLen);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);

	// Forcefully attaching socket to the port 8080
	if (bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0)
	{
		perror("bind failed");
		return false;
	}
	if (listen(serverSocket, 1) < 0)
	{
		perror("listen");
		return false;
	}

	return true;
}

bool TcpLink::open(const char* ip, uint16_t port)
{
	if (clientSocket >= 0)
	{
		return true;
	}

	sockaddr_in clientAddress;
	int addressLength = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, (socklen_t*) &addressLength);
	if (clientSocket < 0)
	{
		perror("accept");
		return false;
	}

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
	open("", 0);
	linkProtocol->resetReceiver();

	char value;
	while (!linkProtocol->isPacketComplete())
	{

		auto readResult = read(clientSocket, &value, 1);
		if (readResult == -1)
		{
			linkProtocol->resetReceiver();
		}
		else if (readResult == 0)
		{
			// server disconnected during read ...
			close(clientSocket);
			clientSocket = -1;

			// reconnect
			open("", 0);

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
