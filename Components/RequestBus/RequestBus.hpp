/*
 * RequestBus.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_
#define COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_

#include <vector>
#include <unordered_map>
#include "BusComponent.hpp"

class RequestBus
{
private:
	std::unordered_map<unsigned int, std::vector<BusComponent*>> components;
public:
	RequestBus();
	virtual ~RequestBus();

	void registerComponent(unsigned int requestType, BusComponent* component);
	void unregisterComponent(unsigned int requestType, BusComponent* component);

	void forwardRequest(std::unique_ptr<Request> request);
};

#endif /* COMPONENTS_REQUESTBUS_REQUESTBUS_HPP_ */
