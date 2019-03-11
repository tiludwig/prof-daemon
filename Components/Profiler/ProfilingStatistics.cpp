/*
 * ProfilingStatistics.cpp
 *
 *  Created on: Mar 11, 2019
 *      Author: timl
 */

#include "ProfilingStatistics.hpp"

void ProfilingStatistics::update(__u64 newValue) {
	_minMax.update(newValue);
	_variance.update(newValue);
}

double ProfilingStatistics::getSampleCount() {
	return _variance.getSampleCount();
}

double ProfilingStatistics::getSampleVariance() {
	return _variance.getSampleVariance();
}

double ProfilingStatistics::getVariance() {
	return _variance.getVariance();
}

double ProfilingStatistics::getMean() {
	return _variance.getMean();
}

__u64 ProfilingStatistics::getMin() {
	return _minMax.getMin();
}

__u64 ProfilingStatistics::getMax() {
	return _minMax.getMax();
}

