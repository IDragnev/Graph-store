#include "CreateGraphCommand.h"
#include "..\StringReader\StringReader.h"
#include "..\..\..\Graph Factory\GraphFactory.h"

const String CreateGraphCommand::DEFAULT_GRAPH_TYPE = "undirected";


void CreateGraphCommand::execute(args::Subparser& parser)
{
	parseArguments(parser);
	execute();
}


void CreateGraphCommand::parseArguments(args::Subparser& parser)
{
	StringPositional id{ parser, "ID", "The ID of the graph to be created" };
	StringPositional type{ parser, "type", "The type of the graph to be created" };
	parser.Parse();
	setIdIfMatched(id);
	setType(type);
}


void CreateGraphCommand::setIdIfMatched(StringPositional& id)
{
	if (id)
	{
		graphID = args::get(id);
	}
	else
	{
		throw std::runtime_error{ "Missing argument: [ID]" };
	}
}


void CreateGraphCommand::setType(StringPositional& type)
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


void CreateGraphCommand::execute() const
{
	std::unique_ptr<Graph> result = GraphFactory::instance().createEmptyGraph(graphType, graphID);
	Command::insertGraph(std::move(result));
	Command::useGraph(graphID);
}


const char* CreateGraphCommand::getName() const
{
	return "CREATE-GRAPH";
}


const char* CreateGraphCommand::getDescription() const
{
	return "Creates a graph of type [type] and ID [ID]";
}