#include "RemoveGraphCommand.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace GS = IDragnev::GraphStore;

static GS::CommandRegistrator<GS::RemoveGraphCommand> registrator;

namespace IDragnev
{
	namespace GraphStore
	{
		void RemoveGraphCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "The ID of the graph to be removed" };
			parser.Parse();
			setGraphID(ID);
		}

		void RemoveGraphCommand::setGraphID(StringPositional& argument)
		{
			if (argument)
			{
				graphID = args::get(argument);
			}
			else
			{
				throw MissingArgument{ argument.Name() };
			}
		}

		void RemoveGraphCommand::execute() const
		{
			Command::removeGraph(graphID);
		}

		const char* RemoveGraphCommand::getName() const
		{
			return "REMOVE-GRAPH";
		}

		const char* RemoveGraphCommand::getDescription() const
		{
			return "Removes a specified graph";
		}
	}
}