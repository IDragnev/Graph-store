#include "UseGraphCommand.h"
#include "Application\Command registrator\CommandRegistrator.h"

namespace IDragnev::GraphStore
{
	static CommandRegistrator<UseGraphCommand> registrator;

	void UseGraphCommand::parseArguments(args::Subparser& parser)
	{
		auto ID = StringPositional{ parser, "ID", "the ID of the graph to be used" };
		parser.Parse();
		setIfMatched(graphID, ID);
	}

	void UseGraphCommand::execute()
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