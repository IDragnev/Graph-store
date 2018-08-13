#include "Application.h"
#include "StringSplitter\StringSplitter.h"


Application& Application::instance()
{
	static Application theOnlyInstance;

	return theOnlyInstance;
}


Application::Application() :
	parser{ "Graph Store application" },
	commandsGroup{ parser, "Commands: " },
	commandsCollection{},
	graphs{},
	receivedExitCommand{ false }
{
	insertExitCommand();
}


void Application::insertExitCommand()
{
	insertCommand("EXIT", "Exits the application", 
		[this](args::Subparser& parser) 
	{ 
		parser.Parse();
		receivedExitCommand = true;
	});
}


void Application::insertCommand(Command& command)
{
	insertCommand(command.getName(), command.getDescription(), [&](args::Subparser& parser) { command.execute(parser); });
}


void Application::insertCommand(const char* name, const char* description, Function coroutine)
{
	assert(name);
	assert(description);

	commandsCollection.emplace_front(commandsGroup, name, description, coroutine);
}


void Application::run()
{
	Command::setManagedStore(graphs);

	while (!receivedExitCommand)
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
		catch (std::runtime_error& e)
		{
			std::cout << e.what() << std::endl << std::endl;
		}
	}
}


Application::InputContainer Application::receiveInput()
{
	StringSplitter<> splitter; 
	const size_t maxInputSize = 512;
	char input[maxInputSize]{ '\0' };

	std::cin.clear();
	std::cin.getline(input, maxInputSize);

	return splitter.split(input);
}