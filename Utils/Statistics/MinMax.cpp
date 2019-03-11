/*
 * MinMax.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#include "MinMax.hpp"

void MinMax::update(__u64 newValue)
{
	if(newValue < _min)
	{
		_min = newValue;
	}

	if(newValue > _max)
	{
		_max = newValue;
	}
}

__u64 MinMax::getMin()
{
	return _min;
}

__u64 MinMax::getMax()
{
	return _max;
}
