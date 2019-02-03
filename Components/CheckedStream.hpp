/*
 * CheckedStream.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_CHECKEDSTREAM_HPP_
#define COMPONENTS_CHECKEDSTREAM_HPP_

#include <vector>

class CheckedStream
{
private:
	std::vector<char> dataBuffer;
	unsigned int streamSum;

private:
	void appendChar(char value);

public:
	CheckedStream(unsigned int size);
	~CheckedStream();

	int getChecksum();
	char* getBuffer();
	bool verifyStream(int checksum);

	template<class T>
	friend CheckedStream& operator<<(CheckedStream& stream, T value);

	friend CheckedStream& operator<<(CheckedStream& stream, const char* value);
	friend CheckedStream& operator<<(CheckedStream& stream, char value);
};


template<class T>
CheckedStream& operator<<(CheckedStream& stream, T value)
{
	for (unsigned int i = 0; i < sizeof(T); i++)
	{
		stream.appendChar(value & 0xFF);
		value = value >> 8;
	}
	return stream;
}

#endif /* COMPONENTS_CHECKEDSTREAM_HPP_ */
