/*
 * DefaultProtocol.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_PROTOCOLLAYER_DEFAULTPROTOCOL_HPP_
#define DATALINK_PROTOCOLLAYER_DEFAULTPROTOCOL_HPP_

#include "LinkProtocol.hpp"
#include "../LinkStream/CheckedLinkStream.hpp"

class DefaultProtocol: public LinkProtocol
{
private:
	enum
	{
		Idle,
		ReadingId,
		ReadingPayloadLength,
		ReadingPayload,
		ReadingChecksum,
		PacketComplete
	} receiverState;
private:
	CheckedLinkStream stream;
	unsigned int payloadToReceive;
public:
	DefaultProtocol();
	virtual ~DefaultProtocol();

	virtual bool isPacketComplete();
	virtual void processData(char data);
	virtual std::unique_ptr<HostPacket> getPacket();
	virtual void resetReceiver();
};

#endif /* DATALINK_PROTOCOLLAYER_DEFAULTPROTOCOL_HPP_ */
