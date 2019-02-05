/*
 * HostPacket.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_HOSTCLIENT_HOSTPACKET_HPP_
#define COMPONENTS_HOSTCLIENT_HOSTPACKET_HPP_

#include "../LinkStream/CheckedLinkStream.hpp"
#include <memory>

class HostPacket
{
private:
	LinkStream payloadStream;
public:
	unsigned int id;
	char checksum;
private:
	HostPacket();
public:
	HostPacket(const HostPacket&) = delete;
	HostPacket(unsigned int id);
	virtual ~HostPacket();

	template <class T>
	void addPayload(T value);

	LinkStream& getPayload();

	unsigned int getPacketSize();

	virtual std::unique_ptr<CheckedLinkStream> serialize();
	static std::unique_ptr<HostPacket> createFromStream(CheckedLinkStream& stream);
	void deserialize(CheckedLinkStream& stream);

	template<class T>
	T createType();
};



//
//	Template definition
//

template <class T>
void HostPacket::addPayload(T value)
{
	payloadStream << value;
}

template<class T>
T HostPacket::createType()
{
	//static_assert(std::is_base_of<IDeserializable, T>::value, "Request::createType<T>(): Type 'T' has to be implement the 'IDeserializable' interface.");
	T concreteType;
	concreteType.deserialize(this->payloadStream);
	return concreteType;
}

#endif /* COMPONENTS_HOSTCLIENT_HOSTPACKET_HPP_ */
