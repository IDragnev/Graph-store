#include "InsertVertexCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<InsertVertexCommand> registrator;


void InsertVertexCommand::parseArguments(args::Subparser& parser)
{
	auto ID = StringPositional{ parser, "ID", "The ID of the vertex to be inserted" };
	parser.Parse();
	setVertexID(ID);
}


void InsertVertexCommand::setVertexID(StringPositional& argument)
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


void InsertVertexCommand::execute() const
{
	auto& used = Command::getUsedGraph();
	used.insertVertexWithID(vertexID);
}


const char* InsertVertexCommand::getName() const
{
	return "INSERT-VERTEX";
}


const char* InsertVertexCommand::getDescription() const
{
	return "Inserts a vertex with specified ID to the currently used graph";
}