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
#include "../Components/Target/RunningThreadTarget.hpp"

#include "Application.hpp"
#include <string>
#include <cstring>
#include <cstdio>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/syscall.h>

using namespace std;

static pid_t threadId;

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

void threadFunc()
{
	threadId = syscall(SYS_gettid);
	printf("My id is: %d\n", threadId);

	while(true)
	{
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		asm volatile("nop");
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	}
}

int Application::run()
{
	std::thread t1(threadFunc);
	try
	{

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		unsigned int argCount = startArgs.getCount();
		if (argCount < 2)
		{
			printUsage();
			return -1;
		}
		//auto threadId = std::hash<std::thread::id>{}(t1.get_id());
		string targetname("./");
		targetname += startArgs[1];
		auto targetArgs = StartupArguments();//startArgs.getRange(1, argCount - 1);
		targetArgs.append("");
		targetArgs.append("");
		auto strThreadId = std::to_string(threadId);
		printf("Target thread id: %d\n", threadId);
		targetArgs.append(strThreadId);

		Profiler prof;
		TcpLink link;

		auto target = std::unique_ptr<RunningThreadTarget>(new RunningThreadTarget());
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
			printf("Got packet\n");
			bus.forwardRequest(std::move(req));
		}

		return 0;
	} catch (const char* err)
	{
		printf("Error: %s\n", err);
	}
	t1.join();
	return -1;
}

void Application::accept(HostPacket* packet)
{
	auto request = packet->createType<AppRequest>();
	exitRequested = request.getValue();
}
