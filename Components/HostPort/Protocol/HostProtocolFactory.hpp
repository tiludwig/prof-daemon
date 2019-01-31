/*
 * HostProtocolFactory.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_HOSTPORT_PROTOCOL_HOSTPROTOCOLFACTORY_HPP_
#define COMPONENTS_HOSTPORT_PROTOCOL_HOSTPROTOCOLFACTORY_HPP_

#include "HostProtocol.hpp"
#include "DefaultProtocol.hpp"

enum class ProtocolType
{
	Default = 0
};

class HostProtocolFactory
{
public:
	static std::unique_ptr<HostProtocol> createProtocol(ProtocolType type)
	{
		std::unique_ptr<HostProtocol> result = nullptr;
		switch(type)
		{
		case ProtocolType::Default:
			result = std::unique_ptr<HostProtocol>(new DefaultProtocol());
			break;
		}
		return result;
	}
};

#endif /* COMPONENTS_HOSTPORT_PROTOCOL_HOSTPROTOCOLFACTORY_HPP_ */
