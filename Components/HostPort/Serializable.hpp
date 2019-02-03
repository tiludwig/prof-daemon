
#ifndef SERIALIZABLE_HPP_
#define SERIALIZABLE_HPP_

#include <memory>

struct SerializedResult
{
	std::unique_ptr<unsigned char[]> buffer;
	unsigned int size;
};

class Serializable
{
public:
	virtual ~Serializable() {}
	virtual SerializedResult serialize() = 0;
};

#endif
