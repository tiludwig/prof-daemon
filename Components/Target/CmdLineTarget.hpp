/*
 * CmdLineTarget.hpp
 *
 *  Created on: Feb 1, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_TARGET_CMDLINETARGET_HPP_
#define COMPONENTS_TARGET_CMDLINETARGET_HPP_

#include "ITarget.hpp"
#include "../../Components/RequestBus/BusComponent.hpp"
#include <sys/types.h>
#include <unistd.h>
#include "../../Components/DataLink/LinkLayer/DataLink.hpp"

class CmdLineTarget: public ITarget, public BusComponent
{
private:
	int readPipe;
	int writePipe;
	__pid_t targetPid;

	std::string filename;
	StartupArguments arguments;

public:

	DataLink* link;
	CmdLineTarget();
	virtual ~CmdLineTarget();

	virtual void initialize();

	virtual void setStartupParameters(const std::string& filename, const StartupArguments& args);

	virtual bool isIsolatedProcess();

	void executeBinary(const std::string& path, StartupArguments& args)
	{
		printf("Target: Executing binary '%s'\n", path.c_str());
		std::vector<char*> arguments;
		arguments.push_back(const_cast<char*>(path.c_str()));

		for(auto& arg : args)
		{
			arguments.push_back(const_cast<char*>(arg.c_str()));
		}
		arguments.push_back(NULL);

		for(unsigned int i = 0; i < arguments.size(); i++)
		{
			printf("Target: Arg%u -> '%s'\n", i, arguments[i]);
		}

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

	virtual void accept(HostPacket* packet);
};

#endif /* COMPONENTS_TARGET_CMDLINETARGET_HPP_ */
