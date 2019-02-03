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
	// TODO Auto-generated constructor stub

}

RequestBus::~RequestBus()
{
	// TODO Auto-generated destructor stub
}

void RequestBus::registerComponent(unsigned int requestType,
		BusComponent* component)
{
	auto iterator = components.find(requestType);
	if(iterator == components.end())
		components[requestType] = std::vector<BusComponent*>();

	components[requestType].push_back(component);
}
void RequestBus::unregisterComponent(unsigned int requestType,
		BusComponent* component)
{
	auto iterator = components.find(requestType);
	if(iterator == components.end())
		return;
	auto componentIterator = std::find(iterator->second.begin(), iterator->second.end(), component);
	iterator->second.erase(componentIterator);
}

void RequestBus::forwardRequest(std::unique_ptr<Request> request)
{
	auto iterator = components.find(request->getType());
	if(iterator == components.end())
		return;

	for(auto& component : iterator->second)
	{
		component->acceptRequest(std::move(request));
	}
}
