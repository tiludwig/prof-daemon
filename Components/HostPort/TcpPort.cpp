/*
 * TcpPort.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#include "TcpPort.hpp"
#include <cstdio>
#include <string.h>

TcpPort::TcpPort()
{
	isInResponseState = false;
}

TcpPort::~TcpPort()
{

}

bool TcpPort::initialize()
{
	return true;
}

std::unique_ptr<Request> TcpPort::waitForRequest()
{
	auto request = std::unique_ptr<Request>(new Request());
	const char* data = "Hello world.";
	request->assignBuffer(strlen(data)+1, const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(data)));
	return request;
}

bool TcpPort::sendResponse(Serializable& response)
{
	if(!isInResponseState)
		throw "[TCPPORT] Not in response state.";

	auto rawResponse = response.serialize();
	printf("Sending response [size=%d, '%s']\n", rawResponse.size, rawResponse.buffer.get());
	return false;
}
