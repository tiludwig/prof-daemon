/*
 * CmdLineTarget.cpp
 *
 *  Created on: Feb 1, 2019
 *      Author: tim
 */

#include "CmdLineTarget.hpp"
#include <sys/wait.h>

CmdLineTarget::CmdLineTarget()
{
	readPipe = -1;
	writePipe = -1;
	targetPid = -1;

	filename = nullptr;
	arguments = nullptr;
}

CmdLineTarget::~CmdLineTarget()
{
}

void CmdLineTarget::initialize()
{
	int pfd[2];
	int pipeResult = pipe(pfd);
	if(pipeResult == -1)
		throw "Failed to create pipe";

	readPipe = pfd[0];
	writePipe = pfd[1];
}

void CmdLineTarget::setStartupParameters(const char* filename, char* arguments[])
{
	this->filename = filename;
	this->arguments = arguments;
}

bool CmdLineTarget::isIsolatedProcess()
{
	return true;
}

__pid_t CmdLineTarget::run()
{
	targetPid = fork();
	if (targetPid == -1)
		throw "Failed to create child process.";

	// check if we are the parent process
	if (targetPid > 0)
		return targetPid;

	// do child stuff
	close (writePipe);
	writePipe = -1;
	// wait for start signal. blocks until parent sends start
	char signal;
	read(readPipe, &signal, 1);
	if (signal != '1')
	{
		throw "Invalid signal from parent process.";
	}

	close (readPipe);
	readPipe = -1;
	execv(filename, arguments);
	// if we reach this point there was an error
	throw "Failed to execute target binary.";
}

void CmdLineTarget::sendData(char* buffer, int count)
{

}

int CmdLineTarget::receiveData(char* buffer, int bytesToRead)
{
	return 0;
}

void CmdLineTarget::startInterestingPart()
{
	// we can close the read pipe
	close(readPipe);
	readPipe = -1;

	// signal start to the target
	write(writePipe, "1", 1);
	close(writePipe);
	writePipe = -1;
}

void CmdLineTarget::waitForTargetToFinish()
{
	waitpid(targetPid, nullptr, 0);
}
