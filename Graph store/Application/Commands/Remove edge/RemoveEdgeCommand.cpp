#include "RemoveEdgeCommand.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<RemoveEdgeCommand> registrator;


void RemoveEdgeCommand::parseArguments(args::Subparser& parser)
{
	StringPositional startID{ parser, "startVertexID", "The ID of the start vertex" };
	StringPositional endID{ parser, "endVertexID", "The ID of the end vertex" };

	parser.Parse();

	setStartIdIfMatched(startID);
	setEndIdIfMatched(endID);
}


void RemoveEdgeCommand::setStartIdIfMatched(StringPositional& startID)
{
	if (startID)
	{
		startVertexID = args::get(startID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [startVertexID]" };
	}
}


void RemoveEdgeCommand::setEndIdIfMatched(StringPositional& endID)
{
	if (endID)
	{
		endVertexID = args::get(endID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [endVertexID]" };
	}
}


void RemoveEdgeCommand::execute() const
{
	Graph& used = Command::getUsedGraph();
	Vertex& start = used.getVertex(startVertexID);
	Vertex& end = used.getVertex(endVertexID);
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