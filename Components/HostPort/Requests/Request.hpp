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
	virtual void deserialize(const class Request&) = 0;
};

class Request
{
private:
	unsigned int type;
	unsigned int length;
	std::unique_ptr<unsigned char[]> buffer;
	unsigned int bufferIndex;
	unsigned int checksum;
private:
	unsigned int calculateChecksum();
public:
	Request();
	~Request();

	void assignBuffer(unsigned int length, std::unique_ptr<unsigned char[]> dataBuffer);
	unsigned char* getBuffer() const;

	void setType(unsigned int type);
	unsigned int getType() const;
	unsigned int getLength() const;

	unsigned int getChecksum() const;
	/**
	 * Creates a concrete request from this generic request
	 */
	template <typename T>
	T createType()
	{
		static_assert(std::is_base_of<IDeserializable, T>::value, "Request::createType<T>(): Type 'T' has to be implement the 'IDeserializable' interface.");
		T concreteType;
		concreteType.deserialize(*this);
		return concreteType;
	}
};

#endif /* HOSTPORT_REQUESTDESERIALIZER_REQUESTS_REQUEST_HPP_ */
