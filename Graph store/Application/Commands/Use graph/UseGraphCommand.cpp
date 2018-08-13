#include "UseGraphCommand.h"


void UseGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	execute();
}


void UseGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional id{ parser, "ID", "the ID of the graph to be used" };
	parser.Parse();
	setIdIfMatched(id);
}


void UseGraphCommand::setIdIfMatched(StringPositional& id)
{
	if (id)
	{
		graphID = args::get(id);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
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