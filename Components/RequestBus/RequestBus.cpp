/*
 * RequestBus.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#include "RequestBus.hpp"
#include <algorithm>

RequestBus::RequestBus()
{
}

RequestBus::~RequestBus()
{
}

void RequestBus::registerComponent(unsigned int requestType, BusComponent* component)
{
	auto iterator = components.find(requestType);
	if (iterator == components.end())
		components[requestType] = std::vector<BusComponent*>();

	components[requestType].push_back(component);
}
void RequestBus::unregisterComponent(unsigned int requestType, BusComponent* component)
{
	auto iterator = components.find(requestType);
	if (iterator == components.end())
		return;
	auto componentIterator = std::find(iterator->second.begin(), iterator->second.end(), component);
	iterator->second.erase(componentIterator);
}

void RequestBus::forwardRequest(std::unique_ptr<HostPacket> packet)
{
	auto iterator = components.find(packet->id);
	if (iterator == components.end())
		return;

	for (auto& component : iterator->second)
	{
		component->accept(packet.get());
	}
}
