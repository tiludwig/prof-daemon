/*
 * Profiler.cpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#include "Profiler.hpp"

class ProfilingRequest: IDeserializable
{
private:
	const char* message;
public:
	ProfilingRequest()
	{
		message = nullptr;
	}

	const char* getMessage()
	{
		return message;
	}

	virtual void deserialize(Request* request)
	{
		message = const_cast<const char*>(reinterpret_cast<char*>(request->getBuffer()));
	}
};

Profiler::Profiler()
{

}

Profiler::~Profiler()
{
}

void Profiler::acceptRequest(Request* base)
{
	auto request = base->createType<ProfilingRequest>();
	printf("[Profiler] Got a request: '%s'\n", request.getMessage());
}
