/*
 * RunningThreadTarget.hpp
 *
 *  Created on: Mar 8, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_TARGET_RUNNINGTHREADTARGET_HPP_
#define COMPONENTS_TARGET_RUNNINGTHREADTARGET_HPP_

#include "ITarget.hpp"
#include "../../Components/RequestBus/BusComponent.hpp"
#include <sys/types.h>
#include <unistd.h>
#include "../../Components/DataLink/LinkLayer/DataLink.hpp"
#include <chrono>

class RunningThreadTarget: public ITarget, public BusComponent
{
private:
	__pid_t threadId;
	std::chrono::steady_clock::time_point startTime;

public:
	DataLink* link;

	virtual ~RunningThreadTarget()
	{
	}

	virtual void initialize();

	virtual void setStartupParameters(const std::string& filename, const StartupArguments& args);

	virtual bool isIsolatedProcess();

	virtual __pid_t run();

	virtual void sendData(char* buffer, int count);

	virtual int receiveData(char* buffer, int bytesToRead);

	virtual void startInterestingPart();
	virtual void waitForTargetToFinish();

	virtual void accept(HostPacket* packet);
};

#endif /* COMPONENTS_TARGET_RUNNINGTHREADTARGET_HPP_ */
