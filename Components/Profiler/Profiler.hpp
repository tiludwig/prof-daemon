/*
 * Profiler.hpp
 *
 *  Created on: Jan 29, 2019
 *      Author: tim
 */

#ifndef COMPONENTS_PROFILER_PROFILER_HPP_
#define COMPONENTS_PROFILER_PROFILER_HPP_

#include "../RequestBus/BusComponent.hpp"

class Profiler : public BusComponent
{
public:
	Profiler();
	virtual ~Profiler();

	virtual void acceptRequest(std::unique_ptr<Request> request);
};

#endif /* COMPONENTS_PROFILER_PROFILER_HPP_ */
