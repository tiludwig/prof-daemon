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

Application::Application()
{
	exitRequested = false;
}

Application::~Application()
{
}

int Application::run(int argc, char** argv)
{
	try
	{
		char* arguments[argc];
		for (int i = 0; i < argc - 1; i++)
		{
			arguments[i] = argv[i + 1];
		}
		arguments[argc - 1] = NULL;

		const char* directory = "./";
		size_t directoryLength = strlen(directory);
		size_t filenameLength = strlen(argv[1]);
		std::string filename;
		filename.reserve(filenameLength + directoryLength);
		filename.append(directory);
		filename.append(argv[1]);

		auto target = std::unique_ptr<CmdLineTarget>(new CmdLineTarget());
		target->setStartupParameters(filename.c_str(), arguments);

		Profiler prof;
		prof.setProfilingTarget(std::move(target));

		auto protocol = std::unique_ptr<DefaultProtocol>(new DefaultProtocol());
		TcpLink link;
		link.setProtocol(std::move(protocol));
		link.initialize();

		prof.link = &link;

		RequestBus bus;
		bus.registerComponent(200, &prof);
		bus.registerComponent(100, this);

		while (!exitRequested)
		{
			auto req = link.waitForPacket();
			bus.forwardRequest(std::move(req));
		}
		printf("Exiting\n");
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
