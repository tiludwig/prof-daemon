/*
 * AppRequest.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#ifndef APPLICATION_REQUESTS_APPREQUEST_HPP_
#define APPLICATION_REQUESTS_APPREQUEST_HPP_

#include "../../Components/DataLink/LinkStream/LinkStream.hpp"

class AppRequest
{
private:
	bool shouldExit;
public:
	AppRequest();

	void deserialize(LinkStream& payload);
	bool getValue();
};

#endif /* APPLICATION_REQUESTS_APPREQUEST_HPP_ */
