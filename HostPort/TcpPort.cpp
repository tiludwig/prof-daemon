/*
 * TcpPort.cpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#include "TcpPort.hpp"
#include <cstdio>

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

bool TcpPort::waitForRequest()
{
	return false;
}

bool TcpPort::sendResponse(Serializable& response)
{
	if(!isInResponseState)
		throw "[TCPPORT] Not in response state.";

	auto rawResponse = response.serialize();
	printf("Sending response [size=%d, '%s']\n", rawResponse.size, rawResponse.buffer.get());
	return false;
}
