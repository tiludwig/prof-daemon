/*
 * RequestBus.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_
#define COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_

#include <vector>
#include "BusComponent.hpp"

class RequestBus
{
private:
	std::vector<BusComponent*> components;
public:
	RequestBus();
	virtual ~RequestBus();

	void registerComponent(unsigned int requestType, BusComponent* component);
	void unregisterComponent(unsigned int requestType, BusComponent* component);

	void forwardRequest(Request* request);
};

#endif /* COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_ */
