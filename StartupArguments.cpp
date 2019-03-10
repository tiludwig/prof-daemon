/*
 * StartupArguments.cpp
 *
 *  Created on: Feb 4, 2019
 *      Author: tim
 */

#include "StartupArguments.hpp"

StartupArguments::StartupArguments()
{

}

void StartupArguments::append(std::string& arg)
{
	arguments.push_back(arg);
}

StartupArguments StartupArguments::getRange(unsigned int start, unsigned int count)
{
	if((start + count) > arguments.size())
		throw "Index out of bounds.";

	StartupArguments args;
	for(unsigned int i = start; i < count; i++)
	{
		args.append(arguments[i]);
	}

	return args;
}

StartupArguments StartupArguments::fromRawArgs(int argc, char** argv)
{
	StartupArguments args;
	for (int i = 0; i < argc; i++)
	{
		args.append(argv[i]);
	}
	return args;
}

void StartupArguments::clear()
{
	arguments.clear();
}

int StartupArguments::getCount()
{
	return arguments.size();
}

void StartupArguments::append(const char* arg)
{
	arguments.push_back(arg);
}

std::vector<std::string>::iterator StartupArguments::begin()
{
	return arguments.begin();
}

std::vector<std::string>::iterator StartupArguments::end()
{
	return arguments.end();
}

std::string& StartupArguments::operator[](unsigned int index)
{
	if (index >= arguments.size())
		throw "Index out of bounds.";

	return arguments[index];
}

const std::string& StartupArguments::operator[](unsigned int index) const
{
	if (index >= arguments.size())
		throw "Index out of bounds.";

	return arguments[index];
}

