#include "Application.h"
#include "..\String\String.h"
#include "Commands\Base\Command.h"
#include "..\Graph\Base Graph\Graph.h"
#include "StringSplitter\StringSplitter.h"
#include "..\General Exceptions\Exception.h"
#include "..\Directory loader\DirectoryLoader.h"
#include <iostream> 


Application& Application::instance()
{
	static Application theOnlyInstance;

	return theOnlyInstance;
}


Application::Application() :
	parser{ "GRAPH STORE APPLICATION" },
	commandsGroup{ parser, "Supported commands" },
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


void Application::run(const String& directory)
{
	try
	{
		load(directory);
		run();
	}
	catch (Exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


void Application::load(const String& directory)
{
	DirectoryLoader loader(directory);

	loader.load([&](std::unique_ptr<Graph> graphPtr)
	{
		graphs.insertGraph(*graphPtr);
		graphPtr.release();
	});

	Command::setManagedStore(graphs);
}


void Application::run()
{
	do
	{
		std::cout << '$';
		invokeCommand(receiveInput());
	} while (!receivedExitCommand);
}


Application::InputContainer Application::receiveInput()
{
	const std::size_t maxInputSize = 512;
	char input[maxInputSize];

	std::cin.clear();
	std::cin.getline(input, maxInputSize);

	StringSplitter<> s{ ' ', '\'' };
	return s.split(input);
}


void Application::invokeCommand(const InputContainer& input)
{
	try
	{
		parser.ParseArgs(input.getBeginConstIterator(), input.getEndConstIterator());
	}
	catch (std::runtime_error& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

