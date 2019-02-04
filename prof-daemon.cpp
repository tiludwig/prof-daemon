/*
 *
 *
 */
#include "Application/Application.hpp"

/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	Application app;
	return app.run(argc, argv);
}
