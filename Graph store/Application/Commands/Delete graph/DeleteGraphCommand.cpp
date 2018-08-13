#include "DeleteGraphCommand.h"


void DeleteGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	execute();
}


void DeleteGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional id{ parser, "ID", "The ID of the graph to be deleted" };
	parser.Parse();
	setIdIfMatched(id);
}


void DeleteGraphCommand::setIdIfMatched(StringPositional& id)
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


void DeleteGraphCommand::execute() const
{
	Command::removeGraph(graphID);
}


const char* DeleteGraphCommand::getName() const
{
	return "DELETE-GRAPH";
}


const char* DeleteGraphCommand::getDescription() const
{
	return "Deletes the specified graph";
}