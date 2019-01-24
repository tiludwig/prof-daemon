/*
 *
 *
 */

#include "HostPort/TcpPort.hpp"
#include <string>
#include <cstring>
#include "HostPort/Request.hpp"

class ProfilingResultResponse: public Serializable
{
private:
	std::string result;
public:
	ProfilingResultResponse(std::string result)
	{
		this->result = result;
	}

	virtual SerializedResult serialize()
	{
		SerializedResult serialized;
		serialized.size = result.size();
		serialized.buffer = std::move(
				std::unique_ptr<char>(new char[serialized.size]));
		memcpy(serialized.buffer.get(), result.c_str(), serialized.size);
		return serialized;
	}
};

class ProfilingRequest
{
public:
	const char* getName()
	{
		return "I am the profiling request!";
	}
};


/**
 * Deserializes a concrete request from the received generic request.
 * The concrete request can be acquired by the getType() function.
 */
class ProfilingRequestDeserializer : public Deserializer<ProfilingRequest>
{
private:
	ProfilingRequest request;
public:
	virtual ProfilingRequest deserialize(Request* type)
	{
		return request;
	}
};

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	TcpPort port;
	ProfilingResultResponse resp("test");

	Request req;

	ProfilingRequestDeserializer deserializer;
	auto request = req.deserializeToType(deserializer);

	printf("%s\n", request.getName());
	bool exitApp = false;
	while(exitApp == false)
	{
		port.waitForRequest();
	}

	return 0;
}
