#include "Application.h"
#include "..\String\String.h"
#include "Commands\Base\Command.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include "..\Directory loader\DirectoryLoader.h"
#include "..\UtilityFunctions.h"
#include <iostream> 

using IDragnev::Utility::print;

namespace IDragnev
{
	namespace GraphStore
	{
		Application& Application::instance()
		{
			static Application theOnlyInstance;

			return theOnlyInstance;
		}

		Application::Application() :
			parser{ "GRAPH STORE APPLICATION" },
			commandsGroup{ parser, "Supported commands" },
			splitter{ '\'', ' ' },
			receivedExitCommand{ false }
		{
			insertExitCommand();
			insertHelpCommand();
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

		void Application::insertHelpCommand()
		{
			insertCommand("HELP", "Lists the supported commands",
			[this](args::Subparser& parser)
			{
				parser.Parse();
				print("Supported commands:\n");
				for (auto&& c : commands)
				{
					print('\t', c.Name(), '\t', c.Help(), '\n');
				}
			});
		}

		void Application::insertCommand(Command& command)
		{
			insertCommand(command.getName(), command.getDescription(), [&command](args::Subparser& p) { command.execute(p); });
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
				print(std::cerr, e.what(), "\n");
			}
		}

		void Application::load(const String& directory)
		{
			auto loader = DirectoryLoader{ directory };

			loader([this](std::unique_ptr<Graph> ptr)
			{
				graphs.insertGraph(std::move(ptr));
			});

			Command::setManagedStore(graphs);
		}

		void Application::run()
		{
			do
			{
				print('$');
				invokeCommand(receiveInput());
			} while (!receivedExitCommand);
		}

		std::string Application::receiveInput()
		{
			const std::size_t maxInputSize = 512;
			char input[maxInputSize];

			std::cin.clear();
			std::cin.getline(input, maxInputSize);

			return input;
		}

		void Application::invokeCommand(const std::string& rawInput)
		{
			try
			{
				auto input = splitter.split(rawInput);
				parser.ParseArgs(input);
			}
			catch (std::runtime_error& e)
			{
				print(std::cerr, e.what(), "\n");
			}
		}
	}
}