/*
 * Profiler.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#include "Profiler.hpp"
#include <cstring>
#include <linux/perf_event.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/ioctl.h>
#include <sys/wait.h>

class ProfilingRequest
{
public:
	bool wasStartRequested;
	std::string targetName;
public:
	ProfilingRequest()
	{
		wasStartRequested = false;
	}

	virtual ~ProfilingRequest()
	{

	}

	bool isStartRequest()
	{
		return wasStartRequested;
	}

	virtual void deserialize(LinkStream& payload)
	{
		payload >> wasStartRequested;
		payload >> targetName;
	}
};

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd, unsigned long flags)
{
	int ret;

	ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
	return ret;
}

Profiler::Profiler()
{
	cycleCounterFd = -1;
	retInstrCounterFd = -1;
	ctxSwitchCounterFd = -1;

	cycleCounterId = 0;
	retInstrCounterId = 0;
	ctxSwitchCounterId = 0;
}

Profiler::~Profiler()
{
	if (cycleCounterFd != -1)
		close(cycleCounterFd);

	if (retInstrCounterFd != -1)
		close(retInstrCounterFd);

	if (ctxSwitchCounterFd != -1)
		close(ctxSwitchCounterFd);
}

void Profiler::initCycleCounter(pid_t pid)
{
	struct perf_event_attr pe;
	//long long count;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_HARDWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_HW_CPU_CYCLES;
	pe.disabled = 1;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;

	if (targetApp->isIsolatedProcess())
	{
		pe.enable_on_exec = 1;
	}

	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	cycleCounterFd = perf_event_open(&pe, pid, -1, -1, 0);
	if (cycleCounterFd == -1)
	{
		printf("initCycleCounter: pid=%d failed to open\n", pid);
		perror("perror: ");
		throw "Failed to open cycle counter.";
	}
}

void Profiler::initRetInstructionsCounter(pid_t pid)
{
	struct perf_event_attr pe;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_HARDWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_HW_INSTRUCTIONS;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	retInstrCounterFd = perf_event_open(&pe, pid, -1, cycleCounterFd, 0);
	if (retInstrCounterFd == -1)
	{
		printf("initRetInstrCounter: pid=%d, leader=%d failed to open\n", pid, cycleCounterFd);
		throw "Failed to open counter for retired instructions.";
	}
}

void Profiler::initCTXSwitchCounter(pid_t pid)
{
	struct perf_event_attr pe;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_SOFTWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_SW_CONTEXT_SWITCHES;
	pe.exclude_kernel = 0;
	pe.exclude_hv = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	ctxSwitchCounterFd = perf_event_open(&pe, pid, -1, cycleCounterFd, 0);
	if (ctxSwitchCounterFd == -1)
	{
		printf("ctxSwitchCounter: pid=%d, leader=%d failed to open\n", pid, cycleCounterFd);
		perror("error: ");
		throw "Failed to open context switch counter.";
	}
}

CounterValues Profiler::readPerformanceCounter()
{
	struct read_format rf;
	CounterValues result;

	read(cycleCounterFd, &rf, sizeof(read_format));
	for (unsigned int i = 0; i < rf.numberOfEvents; i++)
	{
		if (rf.events[i].id == cycleCounterId)
		{
			result.cycleCount = rf.events[i].value;
		}
		else if (rf.events[i].id == retInstrCounterId)
		{
			result.retInstructionsCount = rf.events[i].value;
		}
		else if (rf.events[i].id == ctxSwitchCounterId)
		{
			result.ctxSwitchesCount = rf.events[i].value;
		}
	}

	return result;
}

void Profiler::setProfilingTarget(std::unique_ptr<ITarget> target)
{
	targetApp = std::move(target);
}

CounterValues Profiler::profile()
{
	targetApp->initialize();
	pid_t childPid = targetApp->run();

	printf("targeting pid: %d\n", childPid);

	initCycleCounter(childPid);
	initRetInstructionsCounter(childPid);
	initCTXSwitchCounter(childPid);

	ioctl(cycleCounterFd, PERF_EVENT_IOC_ID, &cycleCounterId);
	ioctl(retInstrCounterFd, PERF_EVENT_IOC_ID, &retInstrCounterId);
	ioctl(ctxSwitchCounterFd, PERF_EVENT_IOC_ID, &ctxSwitchCounterId);

	if (!targetApp->isIsolatedProcess())
	{
		// if it is not an isolated process start the counters now.
		ioctl(cycleCounterFd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
	}

	CounterValues statistic = { 0 };
	CounterValues max = { 0 };
	for (int i = 0; i < 1000; i++)
	{
		ioctl(cycleCounterFd, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
		targetApp->startInterestingPart();
		targetApp->waitForTargetToFinish();
		//ioctl(cycleCounterFd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

		auto results = readPerformanceCounter();
		statistic.ctxSwitchesCount += results.ctxSwitchesCount;
		statistic.cycleCount += results.cycleCount;
		statistic.retInstructionsCount += results.retInstructionsCount;

		if (results.cycleCount > max.cycleCount)
		{
			max.ctxSwitchesCount = results.ctxSwitchesCount;
			max.cycleCount = results.cycleCount;
			max.retInstructionsCount = results.retInstructionsCount;
		}

		printf("%llu %llu %llu\n", results.ctxSwitchesCount, results.cycleCount, results.retInstructionsCount);

	}
	ioctl(cycleCounterFd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
	statistic.ctxSwitchesCount /= 1000;
	statistic.cycleCount /= 1000;
	statistic.retInstructionsCount /= 1000;
	printf("%llu %llu %llu\n", statistic.ctxSwitchesCount, statistic.cycleCount, statistic.retInstructionsCount);

	printf("%llu %llu %llu\n", max.ctxSwitchesCount, max.cycleCount, max.retInstructionsCount);

	return statistic;
}

void Profiler::accept(HostPacket* packet)
{
	auto request = packet->createType<ProfilingRequest>();
	if (request.isStartRequest())
	{
		auto profilingResult = profile();
		ProfilingResponse response(profilingResult);
		link->sendPacket(response);
	}
}
