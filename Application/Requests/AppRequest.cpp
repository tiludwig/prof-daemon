/*
 * AppRequest.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#include "AppRequest.hpp"

AppRequest::AppRequest()
{
	shouldExit = false;
}

void AppRequest::deserialize(LinkStream& payload)
{
	payload >> shouldExit;
}

bool AppRequest::getValue()
{
	return shouldExit;
}
