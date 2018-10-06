#include "RemoveEdgeCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveEdgeCommand> registrator;


void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
{
	auto startID = StringPositional{ parser, "startVertexID", "The ID of the start vertex" };
	auto endID = StringPositional{ parser, "endVertexID", "The ID of the end vertex" };
	parser.Parse();

	setStartVertexID(startID);
	setEndVertexID(endID);
}


void RemoveEdgeCommand::setStartVertexID(StringPositional& argument)
{
	setIfMatched(startVertexID, argument);
}


void RemoveEdgeCommand::setEndVertexID(StringPositional& argument)
{
	setIfMatched(endVertexID, argument);
}


void RemoveEdgeCommand::setIfMatched(String& str, StringPositional& argument)
{
	if (argument)
	{
		str = args::get(argument);
	}
	else
	{
		throw MissingArgument{ argument.Name() };
	}
}


void RemoveEdgeCommand::execute() const
{
	auto& used = Command::getUsedGraph();
	auto& start = used.getVertex(startVertexID);
	auto& end = used.getVertex(endVertexID);
	used.removeEdge(start, end);
}


const char* RemoveEdgeCommand::getName() const
{
	return "REMOVE-EDGE";
}


const char* RemoveEdgeCommand::getDescription() const
{
	return "Removes the edge between a specified pair of vertices in the currently used graph";
}