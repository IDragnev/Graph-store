#include "SearchPathCommand.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include "..\..\..\ShortestPathAlgorithm Store\ShortestPathAlgorithmStore.h"
#include "PathUtilityFunctions.h"

static CommandRegistrator<SearchPathCommand> registrator;


void SearchPathCommand::parseArguments(args::Subparser& parser)
{
	StringPositional startID{ parser, "startVertexID", "The ID of the start of the sought path" };
	StringPositional endID{ parser, "endVertexID", "The ID of the end of the sought path" };
	StringPositional algorithm{ parser, "algorithm", "The algorithm to be used" };

	parser.Parse();

	setStartVertexID(startID);
	setEndVertexID(endID);
	setAlgorithmID(algorithm);
}


void SearchPathCommand::setStartVertexID(StringPositional& startID)
{
	if (startID) 
	{
		startVertexID = args::get(startID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [" + startID.Name() + "]" };
	}
}


void SearchPathCommand::setEndVertexID(StringPositional& endID)
{
	if (endID)
	{
		endVertexID = args::get(endID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [" + endID.Name() + "]" };
	}
}


void SearchPathCommand::setAlgorithmID(StringPositional& algorithm)
{
	if (algorithm)
	{
		algorithmID = args::get(algorithm);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [" + algorithm.Name() + "]" };
	}
}


void SearchPathCommand::execute() const
{
	Graph& usedGraph = Command::getUsedGraph();
	Vertex& start = usedGraph.getVertex(startVertexID);
	Vertex& end = usedGraph.getVertex(endVertexID);
	ShortestPathAlgorithm& algorithm = ShortestPathAlgorithmStore::instance().getAlgorithm(algorithmID);

	algorithm.findShortestPath(usedGraph, start, end);
	printPath(start, end);
	printPathLength(start, end);
}


const char* SearchPathCommand::getName()  const
{
	return "SEARCH-PATH";
}


const char* SearchPathCommand::getDescription() const
{
	return "Searches a shortest path between a specified pair of vertices in the currently used graph with a specified algorithm";
}