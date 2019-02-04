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
#include "../StartupArguments.hpp"

class Application : public BusComponent
{
private:
	bool exitRequested;
	StartupArguments& startArgs;

private:
	void printUsage();
public:
	Application(StartupArguments& args);
	virtual ~Application();

	void initialize();
	int run();

	virtual void accept(HostPacket* packet);
};

#endif /* APPLICATION_APPLICATION_HPP_ */
