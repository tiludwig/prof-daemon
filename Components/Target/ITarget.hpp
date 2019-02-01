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


	/**
	 * @returns the pid of the target process
	 */
	virtual void run(const char* filename, char* arguments[]) = 0;

	/**
	 *
	 */
	virtual void write(char* buffer, int count) = 0;

	/**
	 *
	 */
	virtual int read(char* buffer, int bytesToRead) = 0;

};

#endif
