/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"

#include "Components/HostPort/Requests/Request.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	TcpPort port;

	RequestBus bus;
	Profiler prof;

	bus.registerComponent(0, &prof);

	auto req = port.waitForRequest();


	return 0;
}
