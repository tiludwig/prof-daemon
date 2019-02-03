/*
 * DataLink.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_DATALINK_HPP_
#define DATALINK_DATALINK_HPP_

#include <stdint.h>
#include "../HostPacket/HostPacket.hpp"

class DataLink
{
public:
	virtual ~DataLink()
	{

	}

	virtual bool initialize() = 0;
	virtual bool open(const char* ip, uint16_t port) = 0;

	virtual void sendPacket(HostPacket& packet) = 0;
	virtual std::unique_ptr<HostPacket> waitForPacket() = 0;
};

#endif /* DATALINK_DATALINK_HPP_ */
