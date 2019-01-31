/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"
#include "Components/HostPort/Protocol/HostProtocolFactory.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"
#include "Components/HostPort/Protocol/DefaultProtocol.hpp"

class ApplicationRequest: IDeserializable
{
private:
	bool shouldExit;
public:
	ApplicationRequest()
	{
		shouldExit = false;
	}
	void deserialize(const Request& req)
	{
		if (req.getType() != 100)
			return;

		auto value = *req.getBuffer();
		shouldExit = (value == 0) ? false : true;
	}

	bool getValue()
	{
		return shouldExit;
	}
};

class Application: public BusComponent
{
private:
	bool wasExitRequested;
public:
	Application()
	{
		wasExitRequested = false;
	}

	~Application()
	{
	}

	int run()
	{
		auto protocol = HostProtocolFactory::createProtocol(
				ProtocolType::Default);
		if (protocol == nullptr)
		{
			printf("No protocol found.\n");
			return -1;
		}

		RequestBus bus;
		Profiler prof;
		TcpPort port(std::move(protocol));

		port.initialize();
		bus.registerComponent(200, &prof);
		bus.registerComponent(100, this);

		while (!wasExitRequested)
		{
			auto req = port.waitForRequest();
			bus.forwardRequest(std::move(req));
		}
		printf("Exiting\n");
		return 0;
	}

	virtual void acceptRequest(std::unique_ptr<Request> req)
	{
		auto request = req->createType<ApplicationRequest>();
		wasExitRequested = request.getValue();
	}
};

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	Application app;
	return app.run();
}
