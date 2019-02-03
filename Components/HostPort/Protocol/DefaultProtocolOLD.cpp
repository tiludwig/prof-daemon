/*
 * DefaultProtocol.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#include "DefaultProtocolOLD.hpp"

#include <cstdio>

DefaultProtocol_OLD::DefaultProtocol_OLD()
{
	printf("Creating protocol.\n");
	currentState = ReceiverStates::Idle;
	tempBytesReceived = 0;
	type = 0;
	length = 0;
	checksum = 0;
	data = nullptr;
}

DefaultProtocol_OLD::~DefaultProtocol_OLD()
{
	printf("Destroying protocol.\n");
}

bool DefaultProtocol_OLD::isPacketComplete()
{
	return (currentState == ReceiverStates::PacketReceived);
}

void DefaultProtocol_OLD::appendData(unsigned char newData)
{
	switch (currentState)
	{
	case ReceiverStates::Idle:
		type = 0;
		length = 0;
		checksum = 0;
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
			data.reset(new unsigned char[length]);
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

std::unique_ptr<Request> DefaultProtocol_OLD::getPacket()
{
	auto result = std::unique_ptr<Request>(new Request());
	result->setType(type);
	std::unique_ptr<unsigned char> dataPtr;

	result->assignBuffer(length, std::move(data));
	return result;
}

void DefaultProtocol_OLD::resetReceiver()
{
	currentState = ReceiverStates::Idle;
}
