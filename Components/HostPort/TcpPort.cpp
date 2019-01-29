/*
 * TcpPort.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#include "TcpPort.hpp"
#include <cstdio>
#include <string.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

TcpPort::TcpPort(HostProtocol* proto)
{
	protocol = proto;
	isInResponseState = false;
	serverSocket = -1;
	clientSocket = -1;
}

TcpPort::~TcpPort()
{
	if (clientSocket >= 0)
		close(clientSocket);
	if (serverSocket >= 0)
		close(serverSocket);
}

bool TcpPort::initialize()
{
	// Creating socket file descriptor
	if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		return false;
	}

	const int optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
			(void*) &optVal, optLen);

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(8080);

	// Forcefully attaching socket to the port 8080
	if (bind(serverSocket, (struct sockaddr *) &serverAddress,
			sizeof(serverAddress)) < 0)
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

void TcpPort::waitForClient()
{
	if (clientSocket >= 0)
	{
		return;
	}

	sockaddr_in clientAddress;
	int addressLength = sizeof(clientAddress);
	clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress,
			(socklen_t*) &addressLength);
	if (clientSocket < 0)
	{
		perror("accept");
		return;
	}
}

Request* TcpPort::waitForRequest()
{
	waitForClient();
	protocol->resetReceiver();
	char temp;

	while(!protocol->isPacketComplete())
	{
		int received = read(clientSocket, &temp, 1);
		if(received == -1)
		{
			protocol->resetReceiver();
		}
		else if(received == 0)
		{
			// socket has disconnected
			close(clientSocket);
			clientSocket = -1;
			// wait for new client
			waitForClient();
			protocol->resetReceiver();
		}
		else
		{
			protocol->appendData(temp);
		}
	}

	return protocol->getPacket();
}

bool TcpPort::sendResponse(Serializable& response)
{
	if (!isInResponseState)
		throw "[TCPPORT] Not in response state.";

	auto rawResponse = response.serialize();
	printf("Sending response [size=%d, '%s']\n", rawResponse.size,
			rawResponse.buffer.get());
	return false;
}
