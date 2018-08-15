#include "RemoveVertexCommand.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveVertexCommand> registrator;


void RemoveVertexCommand::parseArguments(args::Subparser& parser)
{
	StringPositional ID{ parser,"ID", "The ID of the vertex to be removed" };
	parser.Parse();
	setIdIfMatched(ID);
}


void RemoveVertexCommand::setIdIfMatched(StringPositional& ID)
{
	if (ID)
	{
		vertexID = args::get(ID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
	}
}


void RemoveVertexCommand::execute() const
{
	Graph& used = Command::getUsedGraph();
	used.removeVertex(vertexID);
}


const char* RemoveVertexCommand::getName() const
{
	return "REMOVE-VERTEX";
}


const char* RemoveVertexCommand::getDescription() const
{
	return "Removes a specified vertex from the currently used graph";
}