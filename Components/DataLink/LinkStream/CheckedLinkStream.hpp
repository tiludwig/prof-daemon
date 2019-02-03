/*
 * CheckedLinkStream.hpp
 *
 *  Created on: Feb 3, 2019
 *      Author: tim
 */

#ifndef DATALINK_CHECKEDLINKSTREAM_HPP_
#define DATALINK_CHECKEDLINKSTREAM_HPP_

#include "LinkStream.hpp"
#include <stdint.h>
class CheckedLinkStream : public LinkStream
{
private:
	uint8_t streamSum;

protected:
	virtual void appendValue(char value);
public:
	CheckedLinkStream();
	CheckedLinkStream(unsigned int initialSize);
	virtual ~CheckedLinkStream();

	char* getBuffer();

	int8_t getChecksum();
	bool verifyStream(int8_t checksum);
};

#endif /* DATALINK_CHECKEDLINKSTREAM_HPP_ */
