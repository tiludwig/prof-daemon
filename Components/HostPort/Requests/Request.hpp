/*
 * Request.hpp
 *
 *  Created on: Jan 24, 2019
 *      Author: tim
 */

#ifndef HOSTPORT_REQUESTDESERIALIZER_REQUESTS_REQUEST_HPP_
#define HOSTPORT_REQUESTDESERIALIZER_REQUESTS_REQUEST_HPP_

#include <type_traits>
#include <memory>

class IDeserializable
{
public:
	virtual ~IDeserializable(){}
	virtual void deserialize(class Request*) = 0;
};

class Request
{
private:
	unsigned int type;
	unsigned int length;
	unsigned char* buffer;
	unsigned int bufferIndex;
	unsigned int checksum;
private:
	unsigned int calculateChecksum();
public:
	Request();
	~Request();

	void assignBuffer(unsigned int length, unsigned char* dataBuffer);
	unsigned char* getBuffer();

	void setType(unsigned int type);
	unsigned int getType();
	unsigned int getLength();

	unsigned int getChecksum();
	/**
	 * Creates a concrete request from this generic request
	 */
	template <typename T>
	T createType()
	{
		static_assert(std::is_base_of<IDeserializable, T>::value, "Request::createType<T>(): Type 'T' has to be implement the 'IDeserializable' interface.");
		T concreteType;
		concreteType.deserialize(this);
		return concreteType;
	}
};

#endif /* HOSTPORT_REQUESTDESERIALIZER_REQUESTS_REQUEST_HPP_ */
