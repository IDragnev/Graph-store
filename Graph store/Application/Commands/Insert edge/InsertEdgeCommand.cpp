#include "InsertEdgeCommand.h"
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


void InsertEdgeCommand::setStartIdIfMatched(StringPositional& startID)
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


void InsertEdgeCommand::setEndIdIfMatched(StringPositional& endID)
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
	return "Inserts an edge with weight [weight] between [startVertexID] and [endVertexID] to the currently used graph";
}