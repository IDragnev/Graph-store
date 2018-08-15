#include "RemoveGraphCommand.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveGraphCommand> registrator;


void RemoveGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional ID{ parser, "ID", "The ID of the graph to be removed" };
	parser.Parse();
	setID(ID);
}


void RemoveGraphCommand::setID(StringPositional& ID)
{
	if (ID)
	{
		graphID = args::get(ID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
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