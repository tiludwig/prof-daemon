/*
 * DefaultProtocol.cpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#include "DefaultProtocol.hpp"

DefaultProtocol::DefaultProtocol()
{
	receiverState = ReadingId;
	payloadToReceive = 0;
}

DefaultProtocol::~DefaultProtocol()
{
}

bool DefaultProtocol::isPacketComplete()
{
	return (receiverState == PacketComplete);
}

void DefaultProtocol::processData(char data)
{
	switch (receiverState)
	{
	case ReadingId:
		stream << data;
		if (stream.streamSize() == 4)
		{
			stream.seekg(0, stream.pos_end);
			receiverState = ReadingPayloadLength;
		}
		break;
	case ReadingPayloadLength:
		stream << data;
		if (stream.bytesAvailable() == 4)
		{
			stream >> payloadToReceive;
			stream.reserve(9 + payloadToReceive);
			receiverState = ReadingPayload;
		}
		break;
	case ReadingPayload:
		stream << data;
		if (--payloadToReceive == 0)
		{
			receiverState = ReadingChecksum;
			stream.seekg(0, stream.pos_end);
		}
		break;
	case ReadingChecksum:
		stream << data;
		if (stream.bytesAvailable() == 1)
		{
			char checksum;
			stream >> checksum;
			if (stream.getChecksum() != 0)
			{
				resetReceiver();
				receiverState = ReadingId;
			}
			else
			{
				receiverState = PacketComplete;
			}
		}
		break;
	default:
		break;
	}
}

std::unique_ptr<HostPacket> DefaultProtocol::getPacket()
{
	stream.seekg(0, stream.pos_beg);
	auto result = HostPacket::createFromStream(stream);
	return std::move(result);
}

void DefaultProtocol::resetReceiver()
{
	stream.reset();
	receiverState = ReadingId;
}

