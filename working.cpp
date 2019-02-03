#include <cstdio>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid, int cpu, int group_fd,
		unsigned long flags)
{
	int ret;

	ret = syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
	return ret;
}

int openCycleCounter(pid_t pid)
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
	pe.enable_on_exec = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	int fd = perf_event_open(&pe, pid, -1, -1, 0);
	return fd;
}

int openInstructionCounter(pid_t pid, int leaderFd)
{
	struct perf_event_attr pe;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_HARDWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_HW_INSTRUCTIONS;
	pe.exclude_kernel = 1;
	pe.exclude_hv = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	int fd = perf_event_open(&pe, pid, -1, leaderFd, 0);
	return fd;
}

int openCTXCounter(pid_t pid, int leaderFd)
{
	struct perf_event_attr pe;
	memset(&pe, 0, sizeof(struct perf_event_attr));
	pe.type = PERF_TYPE_SOFTWARE;
	pe.size = sizeof(struct perf_event_attr);
	pe.config = PERF_COUNT_SW_CONTEXT_SWITCHES;
	pe.exclude_kernel = 0;
	pe.exclude_hv = 1;
	pe.read_format = PERF_FORMAT_GROUP | PERF_FORMAT_ID;

	int fd = perf_event_open(&pe, pid, -1, leaderFd, 0);
	return fd;
}

struct read_format
{
	__u64 nr; /* The number of events */
	//__u64 time_enabled; /* if PERF_FORMAT_TOTAL_TIME_ENABLED */
	//__u64 time_running; /* if PERF_FORMAT_TOTAL_TIME_RUNNING */
	struct
	{
		__u64 value; /* The value of the event */
		__u64 id; /* if PERF_FORMAT_ID */
	} values[3];
};
int workingmain(int argc, char** argv)
{
	int pfd[2];
	int pipeResult = pipe(pfd);
	if (pipeResult == -1)
		return -1;
	pid_t pid = fork();
	if (pid == 0)
	{
		// I am the child
		// close writing file descriptor
		close(pfd[1]);
		printf("child: waiting for start signal\n");
		char signal;
		read(pfd[0], &signal, 1);
		if (signal == '1')
		{
			// close reading file descriptor
			close(pfd[0]);
			printf("child: starting\n");
			// load new executable
			const char *path = "/home/tim/app";
			char * arguments[] =
			{ (char*) "/home/tim/app", (char*) "65536", NULL };
			execv(path, arguments);
			printf("Ooops ... something went badly wrong.\n");
			perror("error: ");
			return -1;
		}
	}
	else if (pid == -1)
	{
		printf("Failed to fork.\n");
		return -2;
	}

	// close reading file descriptor
	close(pfd[0]);

	// setup counters
	int cycleCounterFd = openCycleCounter(pid);
	int instrCounterFd = openInstructionCounter(pid, cycleCounterFd);
	int ctxCounterFd = openCTXCounter(pid, cycleCounterFd);

	__u64 cpuCyclesId, instId, ctxId;
	ioctl(cycleCounterFd, PERF_EVENT_IOC_ID, &cpuCyclesId);
	ioctl(instrCounterFd, PERF_EVENT_IOC_ID, &instId);
	ioctl(ctxCounterFd, PERF_EVENT_IOC_ID, &ctxId);

	//auto start = std::chrono::high_resolution_clock::now();

	// signal start to child
	write(pfd[1], "1", 1);
	// wait until finished
	waitpid(pid, NULL, 0);
	printf("Child finished\n");
	ioctl(cycleCounterFd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
	//auto end = std::chrono::high_resolution_clock::now();
	//auto time_span = std::chrono::duration_cast < std::chrono::microseconds > (end - start);

	uint64_t cpuCycles = 0;
	uint64_t retiredInstructions = 0;
	uint64_t ctxSwitches = 0;

	struct read_format rf;

	read(cycleCounterFd, &rf, sizeof(read_format));
	for (unsigned int i = 0; i < rf.nr; i++)
	{
		if (rf.values[i].id == cpuCyclesId)
		{
			cpuCycles = rf.values[i].value;
		}
		else if (rf.values[i].id == instId)
		{
			retiredInstructions = rf.values[i].value;
		}
		else if (rf.values[i].id == ctxId)
		{
			ctxSwitches = rf.values[i].value;
		}
	}

	close(cycleCounterFd);
	close(ctxCounterFd);
	close(instrCounterFd);
	close(pfd[1]);

	printf("\nProfiling result:\n");
	printf("%20llu cycles\n", cpuCycles);
	printf("%20llu retired instructions\n", retiredInstructions);
	printf("%20.2f cycles per instruction\n", ((float)cpuCycles/retiredInstructions));
	printf("%20llu context switches\n", ctxSwitches);
	printf("%20.2f ms elapsed\n", (cpuCycles/1200000.0));
	return 0;
}
;
