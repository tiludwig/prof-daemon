/*
 *
 *
 */
#include "Application/Application.hpp"
#include "StartupArguments.hpp"
/**
 *	Daemon main entry point
 */
int main(int argc, char** argv)
{
	auto arguments = StartupArguments::fromRawArgs(argc, argv);

	Application app(arguments);
	app.initialize();
	return app.run();
}
