/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"
#include "Components/HostPort/Protocol/HostProtocolFactory.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"
#include "Components/Target/CmdLineTarget.hpp"
#include <cstring>
#include <string>
#include "Components/HostPort/Protocol/DefaultProtocolOLD.hpp"
#include "Components/DataLink/LinkLayer/TcpLink.hpp"
#include "Components/DataLink/ProtocolLayer/DefaultProtocol.hpp"

class ApplicationRequest
{
private:
	bool shouldExit;
public:
	ApplicationRequest()
	{
		shouldExit = false;
	}
	void deserialize(LinkStream& payload)
	{
		payload >> shouldExit;
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

	int run(int argc, char** argv)
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

			while (!wasExitRequested)
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

	virtual void accept(HostPacket* packet)
	{
		auto request = packet->createType<ApplicationRequest>();
		wasExitRequested = request.getValue();
	}
};

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	Application app;
	return app.run(argc, argv);
}
