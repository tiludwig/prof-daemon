/*
 * Application.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#include "../Components/RequestBus/RequestBus.hpp"
#include "../Components/Profiler/Profiler.hpp"
#include "../Components/Target/CmdLineTarget.hpp"
#include "../Components/DataLink/LinkLayer/TcpLink.hpp"
#include "../Components/DataLink/ProtocolLayer/DefaultProtocol.hpp"

#include "Application.hpp"
#include <string>
#include <cstring>

using namespace std;

Application::Application(StartupArguments& args) :
		startArgs(args)
{
	exitRequested = false;
}

Application::~Application()
{
}

void Application::initialize()
{

}

void Application::printUsage()
{
	printf("Usage: pdaemon <target>	<options...>\n");
	printf("\n");
	printf("Arguments:\n");
	printf("       target\tThe name of the target application located in ./targets/");
	printf("\n");
	printf("       options\tThese options will be forwarded to the target.\n");
}

int Application::run()
{
	try
	{
		unsigned int argCount = startArgs.getCount();
		if (argCount < 2)
		{
			printUsage();
			return -1;
		}
		string targetname("./");
		targetname += startArgs[1];
		auto targetArgs = startArgs.getRange(1, argCount - 1);

		Profiler prof;
		TcpLink link;

		auto target = std::unique_ptr<CmdLineTarget>(new CmdLineTarget());
		target->link = &link;
		target->setStartupParameters(targetname, targetArgs);

		RequestBus bus;
		bus.registerComponent(200, &prof);
		bus.registerComponent(100, this);
		bus.registerComponent(300, target.get());

		prof.setProfilingTarget(std::move(target));

		auto protocol = std::unique_ptr<DefaultProtocol>(new DefaultProtocol());
		link.setProtocol(std::move(protocol));
		link.initialize();

		prof.link = &link;


		while (!exitRequested)
		{
			auto req = link.waitForPacket();
			bus.forwardRequest(std::move(req));
		}
		return 0;
	} catch (const char* err)
	{
		printf("Error: %s\n", err);
	}
	return -1;
}

void Application::accept(HostPacket* packet)
{
	auto request = packet->createType<AppRequest>();
	exitRequested = request.getValue();
}
