#include "UseGraphCommand.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<UseGraphCommand> registrator;

		void UseGraphCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "the ID of the graph to be used" };
			parser.Parse();
			setIfMatched(graphID, ID);
		}

		void UseGraphCommand::execute() const
		{
			Command::useGraph(graphID);
		}

		const char* UseGraphCommand::getName() const noexcept
		{
			return "USE-GRAPH";
		}

		const char* UseGraphCommand::getDescription() const noexcept
		{
			return "Changes the used graph";
		}
	}
}