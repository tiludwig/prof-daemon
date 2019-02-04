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

	std::string filename;
	StartupArguments arguments;

public:
	CmdLineTarget();
	virtual ~CmdLineTarget();

	virtual void initialize();

	virtual void setStartupParameters(const std::string& filename, const StartupArguments& args);

	virtual bool isIsolatedProcess();

	void executeBinary(const std::string& path, StartupArguments& args)
	{
		std::vector<char*> arguments;

		for(auto& arg : args)
		{
			arguments.push_back(const_cast<char*>(arg.c_str()));
		}
		arguments.push_back(NULL);

		execv(path.c_str(), &arguments[0]);
	}

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
