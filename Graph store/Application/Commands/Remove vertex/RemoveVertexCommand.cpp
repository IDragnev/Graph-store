#include "RemoveVertexCommand.h"


void RemoveVertexCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	execute();
}


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
	Vertex& v = used.getVertex(vertexID);
	used.removeVertex(v);
}


const char* RemoveVertexCommand::getName() const
{
	return "REMOVE-VERTEX";
}


const char* RemoveVertexCommand::getDescription() const
{
	return "Removes the vertex with ID [ID] from the currently used graph";
}