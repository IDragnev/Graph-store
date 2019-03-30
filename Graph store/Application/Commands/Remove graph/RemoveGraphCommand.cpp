#include "RemoveGraphCommand.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<RemoveGraphCommand> registrator;

		void RemoveGraphCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "The ID of the graph to be removed" };
			parser.Parse();	
			setIfMatched(graphID, ID);
		}

		void RemoveGraphCommand::execute()
		{
			Command::removeGraph(graphID);
		}

		const char* RemoveGraphCommand::getName() const noexcept
		{
			return "REMOVE-GRAPH";
		}
		
		const char* RemoveGraphCommand::getDescription() const noexcept
		{
			return "Removes a specified graph";
		}
	}
}