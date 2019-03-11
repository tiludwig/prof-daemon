/*
 * ProfilingStatistics.hpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#ifndef COMPONENTS_PROFILER_PROFILINGSTATISTICS_HPP_
#define COMPONENTS_PROFILER_PROFILINGSTATISTICS_HPP_

#include "../../Utils/Statistics/Variance.hpp"
#include "../../Utils/Statistics/MinMax.hpp"

class ProfilingStatistics
{
private:
	Variance _variance;
	MinMax _minMax;

public:
	void update(__u64 newValue);

	__u32 getSampleCount();
	double getSampleVariance();
	double getVariance();
	double getMean();
	__u64 getMin();
	__u64 getMax();
};

#endif /* COMPONENTS_PROFILER_PROFILINGSTATISTICS_HPP_ */
