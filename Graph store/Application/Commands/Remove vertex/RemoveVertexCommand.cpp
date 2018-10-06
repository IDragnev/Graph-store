#include "RemoveVertexCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveVertexCommand> registrator;


void RemoveVertexCommand::parseArguments(args::Subparser& parser)
{
	auto ID = StringPositional{ parser,"ID", "The ID of the vertex to be removed" };
	parser.Parse();
	setVertexID(ID);
}


void RemoveVertexCommand::setVertexID(StringPositional& argument)
{
	if (argument)
	{
		vertexID = args::get(argument);
	}
	else
	{
		throw MissingArgument{ argument.Name() };
	}
}


void RemoveVertexCommand::execute() const
{
	auto& used = Command::getUsedGraph();
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