#include "Application.h"
#include "StringSplitter\StringSplitter.h"


Application& Application::instance()
{
	static Application theOnlyInstance;

	return theOnlyInstance;
}


Application::Application() :
	parser{ "GRAPH STORE APPLICATION" },
	commandsGroup{ parser, "Supported commands: " },
	commands{},
	graphs{},
	receivedExitCommand{ false }
{
	insertExitCommand();
	insertHelpCommand();
}


void Application::insertExitCommand()
{
	insertCommand("EXIT", "Exits the application", 
		[&](args::Subparser& parser) 
	{ 
		parser.Parse();
		receivedExitCommand = true;
	});
}


void Application::insertHelpCommand()
{
	insertCommand("HELP", "Lists the supported commands", [&](args::Subparser& parser)
	{
		parser.Parse();
		std::cout << "Supported commands:\n";
		std::for_each(commands.cbegin(), commands.cend(), [&](const args::Command& command)
		{
			std::cout << '\t' << command.Name() << '\t' << command.Help() << '\n';
		});
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

	commands.emplace_front(commandsGroup, name, description, coroutine);
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