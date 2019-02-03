/*
 * HostPacket.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "HostPacket.hpp"

HostPacket::HostPacket()
{
	this->id = 0;
	payloadSize = 0;
	checksum = 0;
	payload = nullptr;
	payloadCreated = false;

}

HostPacket::HostPacket(unsigned int id)
{
	this->id = id;
	payloadSize = 0;
	checksum = 0;
	payload = nullptr;
	payloadCreated = false;
}

HostPacket::~HostPacket()
{
	if (payloadCreated)
	{
		printf("Deleting payload.\n");
		delete[] payload;
	}
}

void HostPacket::addPayload(const char* payload, unsigned int size)
{
	payloadSize = size;
	this->payload = payload;
}

unsigned int HostPacket::getPacketSize()
{
	return (4 + 4 + payloadSize + 1);
}

std::unique_ptr<CheckedLinkStream> HostPacket::serialize()
{
	auto stream = std::unique_ptr<CheckedLinkStream>(new CheckedLinkStream(payloadSize));
	//CheckedLinkStream stream(payloadSize);
	*stream << id;
	*stream << payloadSize;
	for (unsigned int i = 0; i < payloadSize; i++)
	{
		*stream << payload[i];
	}
	*stream << checksum;
	return std::move(stream);
}

void HostPacket::deserialize(CheckedLinkStream& stream)
{
	if (stream.getChecksum() != 0)
		throw "Failed to create HostPacket from CheckedStream (invalid stream).";

	stream >> id;
	stream >> payloadSize;

	char* buffer = new char[payloadSize];
	payloadCreated = true;
	for (unsigned int i = 0; i < payloadSize; i++)
	{
		stream >> buffer[i];
	}
	payload = const_cast<const char*>(buffer);
	stream >> checksum;
}

//
//	static method for deserializing the hostpacket
//
std::unique_ptr<HostPacket> HostPacket::createFromStream(CheckedLinkStream& stream)
{
	auto packet = std::unique_ptr<HostPacket>(new HostPacket());
	packet->deserialize(stream);
	return std::move(packet);
}
