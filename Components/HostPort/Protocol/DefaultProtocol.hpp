/*
 * DefaultProtocol.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_HOSTPORT_PROTOCOL_DEFAULTPROTOCOL_HPP_
#define COMPONENTS_HOSTPORT_PROTOCOL_DEFAULTPROTOCOL_HPP_

#include "../Requests/Request.hpp"
#include "HostProtocol.hpp"

namespace ReceiverStates
{
enum ReceiverState
{
	Idle,
	ReadingType,
	ReadingLength,
	ReadingData,
	ReadingChecksum,
	PacketReceived
};
}

class DefaultProtocol : public HostProtocol
{
private:
	ReceiverStates::ReceiverState currentState;
	unsigned int tempBytesReceived;
	unsigned int type;
	unsigned int length;
	unsigned int checksum;
	std::unique_ptr<unsigned char[]> data;
public:
	DefaultProtocol();
	virtual ~DefaultProtocol();

	virtual bool isPacketComplete();
	virtual void appendData(unsigned char data);
	virtual std::unique_ptr<Request> getPacket();
	virtual void resetReceiver();
};

#endif /* COMPONENTS_HOSTPORT_PROTOCOL_DEFAULTPROTOCOL_HPP_ */
