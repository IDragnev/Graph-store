#include "Command.h"
#include "..\..\..\GraphStore\GraphStore.h"


Graph* Command::usedGraph =  nullptr;
GraphStore* Command::graphStore = nullptr;


void Command::execute(args::Subparser& parser)
{
	parseArguments(parser);
	execute();
}


void Command::setManagedStore(GraphStore& store)
{
	Command::graphStore = &store;
	Command::usedGraph = nullptr;
}


void Command::useGraph(const String& ID)
{
	assert(Command::graphStore);

	Command::usedGraph = &Command::graphStore->getGraph(ID);
}


Graph& Command::getUsedGraph()
{
	if (Command::usedGraph)
	{
		return *Command::usedGraph;
	}
	else
	{
		throw std::runtime_error{ "No graph is currently being used" };
	}
}


void Command::insertGraph(std::unique_ptr<Graph> graphPtr)
{
	assert(Command::graphStore);

	Command::graphStore->insertGraph(*graphPtr);
	graphPtr.release();
}


void Command::removeGraph(const String& ID)
{
	assert(Command::graphStore);

	if (Command::usedGraph && Command::usedGraph->getID() == ID)
	{
		Command::usedGraph = nullptr;
	}

	Command::graphStore->removeGraph(ID);
}