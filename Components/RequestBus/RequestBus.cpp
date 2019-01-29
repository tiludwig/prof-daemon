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
	components.push_back(component);
}
void RequestBus::unregisterComponent(unsigned int requestType,
		BusComponent* component)
{
	auto iterator = std::find(components.begin(), components.end(), component);
	if(iterator == components.end())
		return;

	components.erase(iterator);
}

void RequestBus::forwardRequest(Request* request)
{
	for(auto& component : components)
		component->acceptRequest(request);
}
