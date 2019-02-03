/*
 * LinkProtocol.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_PROTOCOLLAYER_LINKPROTOCOL_HPP_
#define DATALINK_PROTOCOLLAYER_LINKPROTOCOL_HPP_

#include "../HostPacket/HostPacket.hpp"

class LinkProtocol
{
public:
	virtual ~LinkProtocol()
	{

	}

	virtual bool isPacketComplete() = 0;
	virtual void processData(char data) = 0;
	virtual std::unique_ptr<HostPacket> getPacket() = 0;
	virtual void resetReceiver() = 0;
};

#endif /* DATALINK_PROTOCOLLAYER_LINKPROTOCOL_HPP_ */
