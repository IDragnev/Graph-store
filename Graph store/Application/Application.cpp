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
	//load...
	Command::setManagedStore(graphs);

	do
	{
		std::cout << '>';
		invokeCommand(receiveInput());
	} while (!receivedExitCommand);
}


Application::InputContainer Application::receiveInput()
{
	const size_t maxInputSize = 512;
	char input[maxInputSize];

	std::cin.clear();
	std::cin.getline(input, maxInputSize);

	StringSplitter<> s; 
	return s.split(input);
}


void Application::invokeCommand(const InputContainer& input)
{
	try
	{
		parser.ParseArgs(input);
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

