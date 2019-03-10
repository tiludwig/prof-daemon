/*
 * StartupArguments.hpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#ifndef STARTUPARGUMENTS_HPP_
#define STARTUPARGUMENTS_HPP_

#include <vector>
#include <string>

class StartupArguments
{
private:
	std::vector<std::string> arguments;
public:
	StartupArguments();

	int getCount();
	void append(const char* arg);
	void append(std::string& arg);

	void clear();

	std::vector<std::string>::iterator begin();
	std::vector<std::string>::iterator end();

	std::string& operator[](unsigned int index);
	const std::string& operator[](unsigned int index) const;

	StartupArguments getRange(unsigned int start, unsigned int count);

	static StartupArguments fromRawArgs(int argc, char** argv);
};

#endif /* STARTUPARGUMENTS_HPP_ */
