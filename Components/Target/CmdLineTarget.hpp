/*
 * CmdLineTarget.hpp
 *
 *  Created on: Feb 1, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_TARGET_CMDLINETARGET_HPP_
#define COMPONENTS_TARGET_CMDLINETARGET_HPP_

#include "ITarget.hpp"
#include <sys/types.h>
#include <unistd.h>

class CmdLineTarget: public ITarget
{
private:
	int readPipe;
	int writePipe;
	__pid_t targetPid;

	const char* filename;
	char** arguments;

public:
	CmdLineTarget();
	virtual ~CmdLineTarget();

	virtual void initialize();

	virtual void setStartupParameters(const char* filename, char* arguments[]);

	virtual bool isIsolatedProcess();

	/**
	 * @returns the pid of the target process
	 */
	virtual __pid_t run();

	/**
	 *
	 */
	virtual void sendData(char* buffer, int count);

	/**
	 *
	 */
	virtual int receiveData(char* buffer, int bytesToRead);

	virtual void startInterestingPart();
	virtual void waitForTargetToFinish();
};

#endif /* COMPONENTS_TARGET_CMDLINETARGET_HPP_ */
