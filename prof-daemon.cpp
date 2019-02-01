/*
 *
 *
 */
#include "Components/HostPort/TcpPort.hpp"
#include "Components/HostPort/Protocol/HostProtocolFactory.hpp"
#include "Components/RequestBus/RequestBus.hpp"
#include "Components/Profiler/Profiler.hpp"
#include "Components/HostPort/Protocol/DefaultProtocol.hpp"
#include "Components/Target/CmdLineTarget.hpp"

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
	try
	{
		Profiler prof;
		char* arguments[argc];
		for(int i = 0; i < argc - 1;i++)
		{
			arguments[i] = argv[i+1];
		}
		arguments[argc-1] = NULL;

		auto target = std::unique_ptr<CmdLineTarget>(new CmdLineTarget());
		target->setStartupParameters(argv[1], arguments);

		prof.setProfilingTarget(std::move(target));
		auto result = prof.profile();

		printf("\nProfiling result:\n");
		printf("%20llu cycles\n", result.cycleCount);
		printf("%20llu retired instructions\n", result.retInstructionsCount);
		printf("%20.2f cycles per instruction\n",
				((float) result.cycleCount / result.retInstructionsCount));
		printf("%20llu context switches\n", result.ctxSwitchesCount);

		double timeElapsed = (result.cycleCount / 1.2);
		int unitIndex = 0;
		const char* units[] =
		{ "ns", "us", "ms", "s" };
		if (timeElapsed > 1000.0)
		{
			timeElapsed = timeElapsed / 1000.0;
			unitIndex++;
		}

		if (timeElapsed > 1000.0)
		{
			timeElapsed = timeElapsed / 1000.0;
			unitIndex++;
		}

		if (timeElapsed > 1000.0)
		{
			timeElapsed = timeElapsed / 1000.0;
			unitIndex++;
		}

		printf("%20.2f %s elapsed\n", timeElapsed, units[unitIndex]);
	} catch (const char* errormsg)
	{
		printf("error: %s\n", errormsg);
		//Application app;
		//return app.run();
	}
}
