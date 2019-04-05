#include "Application.h"
#include "..\String\String.h"
#include "Commands\Base\Command.h"
#include "..\Graph\Base Graph\Graph.h"
#include "..\General Exceptions\Exception.h"
#include "..\Directory loader\DirectoryLoader.h"
#include "..\UtilityFunctions.h"
#include "..\..\Third party\fmt-5.3.0\include\fmt\format.h"
#include "..\StringSplitter\StringSplitter.h"
#include <iostream> 

namespace fs = std::filesystem;
using IDragnev::Utility::print;
using namespace fmt::literals;

namespace IDragnev
{
	namespace GraphStore
	{
		class FailedToSetDirectory : public Exception 
		{
		public:
			FailedToSetDirectory(const String& path) :
				Exception{ fmt::format("Failed to set working directory to {}", path) }
			{
			}
		};

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
			auto list = [](const auto& c) { fmt::print(" - {name:<15} {descr}\n", "name"_a = c.Name(), "descr"_a = c.Help()); };

			insertCommand("HELP", "Lists the supported commands",
			[this, list](args::Subparser& parser)
			{
				parser.Parse();
				print("Supported commands:\n");
				std::for_each(commands.cbegin(), commands.cend(), list);
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
				setWorkingDirectory(directory);
				load(directory);
				run();
			}
			catch (Exception& e)
			{
				print(std::cerr, e.what(), "\n");
			}
		}

		void Application::setWorkingDirectory(const String& path)
		{
			try
			{
				fs::current_path(static_cast<const char*>(path));
			}
			catch (fs::filesystem_error&)
			{
				throw FailedToSetDirectory{ path };
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
				parser.ParseArgs(splitter(rawInput));
			}
			catch (std::runtime_error& e)
			{
				print(std::cerr, e.what(), "\n");
			}
		}
	}
}