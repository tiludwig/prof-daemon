/*
 * Profiler.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_PROFILER_PROFILER_HPP_
#define COMPONENTS_PROFILER_PROFILER_HPP_

#include "../RequestBus/BusComponent.hpp"
#include <linux/types.h>

struct CounterValues
{
	__u64 cycleCount;
	__u64 retInstructionsCount;
	__u64 ctxSwitchesCount;
};

struct read_format
{
	__u64 numberOfEvents; /* The number of events */
	//__u64 time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
	//__u64 time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
	struct
	{
		__u64 value;
		__u64 id;
	} events[3];
};

class Profiler: public BusComponent
{
private:
	pid_t targetPid;
	int cycleCounterFd;
	int retInstrCounterFd;
	int ctxSwitchCounterFd;

	int childReadPipe;
	int childWritePipe;

	__u64 cycleCounterId;
	__u64 retInstrCounterId;
	__u64 ctxSwitchCounterId;
private:
	void initCycleCounter(pid_t pid);
	void initRetInstructionsCounter(pid_t pid);
	void initCTXSwitchCounter(pid_t pid);

	CounterValues readPerformanceCounter();

	void createChildProcessPipe();
	pid_t startChildProcess(const char* filename, char** arguments);
public:
	Profiler();
	virtual ~Profiler();

	CounterValues profile(const char* filename, char** arguments);

	virtual void acceptRequest(std::unique_ptr<Request> request);
};

#endif /* COMPONENTS_PROFILER_PROFILER_HPP_ */
