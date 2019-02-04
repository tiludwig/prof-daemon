/*
 * Application.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#ifndef APPLICATION_APPLICATION_HPP_
#define APPLICATION_APPLICATION_HPP_

#include "Requests/AppRequest.hpp"
#include "../Components/RequestBus/BusComponent.hpp"

class Application : public BusComponent
{
private:
	bool exitRequested;

public:
	Application();
	virtual ~Application();

	int run(int argc, char** argv);

	virtual void accept(HostPacket* packet);
};

#endif /* APPLICATION_APPLICATION_HPP_ */
