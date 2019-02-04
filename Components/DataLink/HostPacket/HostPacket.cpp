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
	checksum = 0;
}

HostPacket::HostPacket(unsigned int id)
{
	this->id = id;
	checksum = 0;
}

HostPacket::~HostPacket()
{
}

unsigned int HostPacket::getPacketSize()
{
	return (4 + 4 + payloadStream.streamSize() + 1);
}

std::unique_ptr<CheckedLinkStream> HostPacket::serialize()
{
	auto stream = std::unique_ptr<CheckedLinkStream>(new CheckedLinkStream(payloadStream.streamSize()));
	*stream << id;
	*stream << payloadStream.streamSize();
	*stream << payloadStream;
	*stream << stream->getChecksum();
	return std::move(stream);
}

void HostPacket::deserialize(CheckedLinkStream& stream)
{
	if (stream.getChecksum() != 0)
		throw "Failed to create HostPacket from CheckedStream (invalid stream).";

	stream >> id;
	unsigned int payloadSize;
	stream >> payloadSize;


	char temp;
	for (unsigned int i = 0; i < payloadSize; i++)
	{
		stream >> temp;
		payloadStream << temp;
	}
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
