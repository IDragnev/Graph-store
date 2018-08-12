#include "Application.h"
#include "StringSplitter\StringSplitter.h"


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


void Application::run()
{
	Command::setManagedStore(graphs);

	while (!shouldExit)
	{
		try
		{
			std::cout << '>';
			parser.ParseArgs(receiveInput());
			std::cout << std::endl;
		}
		catch (args::Help)
		{
			std::cout << parser;
		}
		catch (args::Error& e)
		{
			std::cerr << e.what() << std::endl << parser;
		}
	}
}


Application::InputContainer Application::receiveInput()
{
	StringSplitter<> splitter;
	std::string input;

	std::cin.clear();
	std::cin >> input;

	return splitter.split(input);
}