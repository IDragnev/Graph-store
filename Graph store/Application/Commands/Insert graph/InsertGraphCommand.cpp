#include "InsertGraphCommand.h"
#include "..\..\..\Graph Factory\GraphFactory.h"
#include "..\..\Command registrator\CommandRegistrator.h"

static CommandRegistrator<InsertGraphCommand> registrator;


const String InsertGraphCommand::DEFAULT_GRAPH_TYPE{ "undirected" };


void InsertGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional ID{ parser, "ID", "The ID of the graph to be created" };
	StringPositional type{ parser, "type", "The type of the graph to be created" };

	parser.Parse();

	setID(ID);
	setType(type);
}


void InsertGraphCommand::setID(StringPositional& ID)
{
	if (ID)
	{
		graphID = args::get(ID);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
	}
}


void InsertGraphCommand::setType(StringPositional& type)
{
	if (type)
	{
		graphType = args::get(type);
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
	return "Inserts a new graph of specified type and ID";
}