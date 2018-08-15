#include "InsertGraphCommand.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\..\Graph Factory\GraphFactory.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include "..\MissingArgument exception\MissingArgument.h"
#include <memory>

static CommandRegistrator<InsertGraphCommand> registrator;


const String InsertGraphCommand::DEFAULT_GRAPH_TYPE{ "undirected" };


void InsertGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional ID{ parser, "ID", "The ID of the graph to be created" };
	StringPositional type{ parser, "type", "The type of the graph to be created" };
	parser.Parse();

	setGraphID(ID);
	setGraphType(type);
}


void InsertGraphCommand::setGraphID(StringPositional& argument)
{
	if (argument)
	{
		graphID = args::get(argument);
	}
	else
	{
		throw MissingArgument{ argument.Name() };
	}
}


void InsertGraphCommand::setGraphType(StringPositional& argument)
{
	if (argument)
	{
		graphType = args::get(argument);
	}
	else
	{
		graphType = DEFAULT_GRAPH_TYPE;
	}
}


void InsertGraphCommand::execute() const
{
	std::unique_ptr<Graph> result = GraphFactory::instance().createEmptyGraph(graphType, graphID);
	Command::insertGraph(std::move(result));
	Command::useGraph(graphID);
}


const char* InsertGraphCommand::getName() const
{
	return "INSERT-GRAPH";
}


const char* InsertGraphCommand::getDescription() const
{
	return "Inserts a new graph with specified ID and type";
}