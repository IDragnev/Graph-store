#include "UseGraphCommand.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

namespace GS = IDragnev::GraphStore;

static GS::CommandRegistrator<GS::UseGraphCommand> registrator;

namespace IDragnev
{
	namespace GraphStore
	{
		void UseGraphCommand::parseArguments(args::Subparser& parser)
		{
			auto ID = StringPositional{ parser, "ID", "the ID of the graph to be used" };
			parser.Parse();
			setGraphID(ID);
		}

		void UseGraphCommand::setGraphID(StringPositional& argument)
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

		void UseGraphCommand::execute() const
		{
			Command::useGraph(graphID);
		}

		const char* UseGraphCommand::getName() const
		{
			return "USE-GRAPH";
		}

		const char* UseGraphCommand::getDescription() const
		{
			return "Changes the used graph";
		}
	}
}