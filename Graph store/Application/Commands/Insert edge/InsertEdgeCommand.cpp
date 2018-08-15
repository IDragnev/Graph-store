#include "InsertEdgeCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<InsertEdgeCommand> registrator;


void InsertEdgeCommand::parseArguments(args::Subparser& parser)
{
	StringPositional startID{ parser, "startVertexID", "The ID of the start vertex" };
	StringPositional endID{ parser, "endVertexID", "The ID of the end vertex" };
	UnsignedPositional weight{ parser, "weight", "The weight of the edge" };

	parser.Parse();

	setStartIdIfMatched(startID);
	setEndIdIfMatched(endID);
	setWeight(weight);	
}


void InsertEdgeCommand::setStartIdIfMatched(StringPositional& argument)
{
	setIfMatched(startVertexID, argument);
}


void InsertEdgeCommand::setEndIdIfMatched(StringPositional& argument)
{
	setIfMatched(endVertexID, argument);
}


void InsertEdgeCommand::setIfMatched(String& str, StringPositional& argument)
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


void InsertEdgeCommand::setWeight(UnsignedPositional& weight)
{
	if (weight)
	{
		this->weight = args::get(weight);
	}
	else
	{
		this->weight = DEFAULT_EDGE_WEIGHT;
	}
}


void InsertEdgeCommand::execute() const
{
	Graph& used = Command::getUsedGraph();
	Vertex& start = used.getVertex(startVertexID);
	Vertex& end = used.getVertex(endVertexID);
	used.insertEdge(start, end, weight);
}


const char* InsertEdgeCommand::getName() const
{
	return "INSERT-EDGE";
}


const char* InsertEdgeCommand::getDescription() const
{
	return "Inserts an edge with specified weight between a specified pair of vertices in the currently used graph";
}