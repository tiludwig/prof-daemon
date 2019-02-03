/*
 * LinkStream.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_LINKSTREAM_HPP_
#define DATALINK_LINKSTREAM_HPP_

#include <vector>

class LinkStream
{
protected:
	std::vector<char> dataBuffer;
	unsigned int streamReadPosition;
public:
	const unsigned int pos_end = 0;
	const unsigned int pos_beg = 1;
	const unsigned int pos_cur = 2;
protected:
	virtual void appendValue(char value);
public:
	LinkStream();
	LinkStream(unsigned int initialSize);
	virtual ~LinkStream();

	void reserve(unsigned int size);
	void reset();

	unsigned int bytesAvailable();
	unsigned int streamSize();

	char* begin();
	char* end();

	void seekg(unsigned int position, unsigned int direction);

	// Stream operators
	template<class T>
	friend LinkStream& operator<<(LinkStream& stream, T value);

	friend LinkStream& operator<<(LinkStream& stream, char value);
	friend LinkStream& operator<<(LinkStream& stream, unsigned char value);
	friend LinkStream& operator<<(LinkStream& stream, const char* value);

	template<class T>
	friend LinkStream& operator>>(LinkStream& stream, T& value);

	friend LinkStream& operator>>(LinkStream& stream, char& value);
	friend LinkStream& operator>>(LinkStream& stream, const char* value);
};


//
//	Template stream operators
//

template<class T>
LinkStream& operator<<(LinkStream& stream, T value)
{
	for (unsigned int i = 0; i < sizeof(T); i++)
	{
		stream.appendValue(value & 0xFF);
		value = value >> 8;
	}
	return stream;
}

template<class T>
LinkStream& operator>>(LinkStream& stream, T& value)
{
	if (stream.bytesAvailable() < sizeof(T))
		throw "Failed to extract from CheckedStream. Not enough bytes available";

	value = 0;
	for (unsigned int i = 0; i < sizeof(value); i++)
	{
		unsigned char temp = static_cast<unsigned char>(stream.dataBuffer[stream.streamReadPosition++]);
		value = value | (temp << 8 * i);
	}
	return stream;
}

#endif /* DATALINK_LINKSTREAM_HPP_ */
