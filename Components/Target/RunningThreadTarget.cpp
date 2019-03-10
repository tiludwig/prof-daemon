/*
 * RunningThreadTarget.cpp
 *
 *  Created on: Mar 8, 2019
 *      Author: tim
 */

#include "RunningThreadTarget.hpp"
#include <thread>
#include <string>


void RunningThreadTarget::initialize()
{

}

void RunningThreadTarget::setStartupParameters(const std::string& filename, const StartupArguments& args)
{
	threadId = std::stoi(args[2]);
}

bool RunningThreadTarget::isIsolatedProcess()
{
	return false;
}

__pid_t RunningThreadTarget::run()
{
	return threadId;
}

void RunningThreadTarget::sendData(char* buffer, int count)
{
	// TBD
}

int RunningThreadTarget::receiveData(char* buffer, int bytesToRead)
{
	// TBD
	return -1;
}

void RunningThreadTarget::startInterestingPart()
{
	// Send start signal to thread pipe
	startTime = std::chrono::steady_clock::now();
}

void RunningThreadTarget::waitForTargetToFinish()
{
	const int msToSleep = 3000;
	int msRemaining = msToSleep;
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
	while(msRemaining > 0)
	{
		// calculate sleep time
		std::this_thread::sleep_for(std::chrono::milliseconds(msRemaining));

		now = std::chrono::steady_clock::now();
		duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
		msRemaining -= duration;
	}
}

void RunningThreadTarget::accept(HostPacket* packet)
{
	HostPacket response(350);
	response.addPayload(true);
	link->sendPacket(response);
}
