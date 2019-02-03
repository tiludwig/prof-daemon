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

	int run(int argc, char** argv)
	{
		try
		{
			auto protocol = HostProtocolFactory::createProtocol(
					ProtocolType::Default);
			if (protocol == nullptr)
			{
				printf("No protocol found.\n");
				return -1;
			}

			Profiler prof;

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

			prof.setProfilingTarget(std::move(target));

			RequestBus bus;
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
		} catch (const char* err)
		{
			printf("Error: %s\n", err);
		}
		return -1;
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
	return app.run(argc, argv);
}
