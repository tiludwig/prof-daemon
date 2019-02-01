#ifndef ITARGET_HPP_
#define ITARGET_HPP_

#include <sys/types.h>
#include <unistd.h>

class ITarget
{
public:
	virtual ~ITarget()
	{
	}

	/**
	 * Initialize the target
	 */
	virtual void initialize() = 0;

	virtual void setStartupParameters(const char* filename, char* arguments[]) = 0;

	virtual bool isIsolatedProcess() = 0;

	/**
	 * @returns the pid of the target process
	 */
	virtual __pid_t run() = 0;

	/**
	 *
	 */
	virtual void sendData(char* buffer, int count) = 0;

	/**
	 *
	 */
	virtual int receiveData(char* buffer, int bytesToRead) = 0;

	virtual void startInterestingPart() = 0;
	virtual void waitForTargetToFinish() = 0;
};

#endif
