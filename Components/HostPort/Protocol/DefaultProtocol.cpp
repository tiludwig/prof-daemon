/*
 * DefaultProtocol.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#include "DefaultProtocol.hpp"
#include <cstdio>

DefaultProtocol::DefaultProtocol()
{
	printf("Creating protocol.\n");
	currentState = ReceiverStates::Idle;
	tempBytesReceived = 0;
	type = 0;
	length = 0;
	checksum = 0;
	data = nullptr;
}

DefaultProtocol::~DefaultProtocol()
{
	printf("Destroying protocol.\n");
}

bool DefaultProtocol::isPacketComplete()
{
	return (currentState == ReceiverStates::PacketReceived);
}

void DefaultProtocol::appendData(unsigned char newData)
{
	switch (currentState)
	{
	case ReceiverStates::Idle:
		type = 0;
		length = 0;
		checksum = 0;
		if (this->data != nullptr)
			delete[] this->data;

		// read first byte
		type = ((unsigned int)newData) << 24;
		tempBytesReceived = 1;
		currentState = ReceiverStates::ReadingType;
		break;
	case ReceiverStates::ReadingType:
		type = type >> 8 | (((unsigned int)newData) << 24);
		tempBytesReceived++;

		if (tempBytesReceived >= sizeof(unsigned int))
		{
			tempBytesReceived = 0;
			currentState = ReceiverStates::ReadingLength;
		}
		break;
	case ReceiverStates::ReadingLength:
		length = length >> 8 | (((unsigned int)newData) << 24);
		tempBytesReceived++;

		if (tempBytesReceived >= sizeof(unsigned int))
		{
			this->data = new unsigned char[length];
			tempBytesReceived = 0;
			currentState = ReceiverStates::ReadingData;
		}
		break;
	case ReceiverStates::ReadingData:
		data[tempBytesReceived++] = newData;
		if (tempBytesReceived == length)
		{
			tempBytesReceived = 0;
			currentState = ReceiverStates::ReadingChecksum;
		}
		break;
	case ReceiverStates::ReadingChecksum:
		checksum = checksum >> 8 | (((unsigned int)newData) << 24);
		tempBytesReceived++;

		if (tempBytesReceived >= sizeof(unsigned int))
		{
			tempBytesReceived = 0;
			currentState = ReceiverStates::PacketReceived;
		}
		break;
	default:
		break;
	}
}

Request* DefaultProtocol::getPacket()
{
	Request* result = new Request();
	result->setType(type);
	result->assignBuffer(length, data);
	return result;
}

void DefaultProtocol::resetReceiver()
{
	currentState = ReceiverStates::Idle;
}
