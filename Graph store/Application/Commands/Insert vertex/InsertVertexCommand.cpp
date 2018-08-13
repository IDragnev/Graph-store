#include "InsertVertexCommand.h"


void InsertVertexCommand::parseArguments(args::Subparser& parser)
{
	StringPositional id{ parser, "ID", "The ID of the vertex to be inserted" };
	parser.Parse();
	setIdIfMatched(id);
}


void InsertVertexCommand::setIdIfMatched(StringPositional& id)
{
	if (id)
	{
		vertexID = args::get(id);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
	}
}


void InsertVertexCommand::execute() const
{
	Graph& used = Command::getUsedGraph();
	used.insertVertexWithID(vertexID);
}


const char* InsertVertexCommand::getName() const
{
	return "INSERT-VERTEX";
}


const char* InsertVertexCommand::getDescription() const
{
	return "Inserts a vertex with ID [ID] to the currently used graph";
}