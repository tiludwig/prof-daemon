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

StartupArguments StartupArguments::fromRawArgs(int argc, char** argv)
{
	StartupArguments args;
	for (int i = 0; i < argc; i++)
	{
		args.append(argv[i]);
	}
	return args;
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

