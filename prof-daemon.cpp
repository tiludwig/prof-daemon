/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"

#include "Components/HostPort/Requests/Request.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"
#include "Components/HostPort/Protocol/DefaultProtocol.hpp"

void displayRequest(Request* request)
{
	printf("[Request] type=%u, length=%u\n\t", request->getType(),
			request->getLength());
	printf("\tdata='%s',\n\t", request->getBuffer());
	printf("\tchecksum=%u\n", request->getChecksum());
}

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	/*TcpPort port;

	 RequestBus bus;
	 Profiler prof;

	 bus.registerComponent(0, &prof);

	 auto req = port.waitForRequest();*/

	DefaultProtocol proto;
	TcpPort port(&proto);
	port.initialize();
	Request* req = nullptr;
	do
	{
		if(req == nullptr)
			delete req;
		req = port.waitForRequest();
		if (req == nullptr)
			break;
		displayRequest(req);
	} while (req->getType() != 100);
	printf("Exiting");
	return 0;
}
