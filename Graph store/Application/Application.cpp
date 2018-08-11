#include "Application.h"


Application& Application::instance()
{
	static Application theOnlyInstance;

	return theOnlyInstance;
}


Application::Application() :
	parser("Graph Store"),
	commandsGroup(parser, "Commands: "),
	commandsCollection(),
	graphs(),
	shouldExit(false)
{
	insertExitCommand();
}


void Application::insertExitCommand()
{
	insertCommand("EXIT", "Exits the application", 
		[this](args::Subparser& parser) 
	{ 
		parser.Parse();
		shouldExit = true;
	});
}


void Application::insertCommand(Command& command)
{
	insertCommand(command.getName(), command.getDescription(), [&](args::Subparser& parser) { command.execute(parser); });
}


void Application::insertCommand(const char* name, const char* description, Function coroutine)
{
	commandsCollection.emplace_front(commandsGroup, name, description, coroutine);
}
