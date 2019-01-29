/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"
#include "Components/HostPort/Protocol/HostProtocolFactory.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"
#include "Components/HostPort/Protocol/DefaultProtocol.hpp"

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	auto protocol = HostProtocolFactory::createProtocol(ProtocolType::Default);
	if(protocol == nullptr)
	{
		printf("No protocol found.\n");
		return -1;
	}
	TcpPort port(protocol);
	port.initialize();

	RequestBus bus;
	Profiler prof;

	bus.registerComponent(0, &prof);
	bool wasExitRequested = false;
	while (!wasExitRequested)
	{
		auto req = port.waitForRequest();
		bus.forwardRequest(req);
		delete req;
	}
	printf("Exiting");
	return 0;
}
