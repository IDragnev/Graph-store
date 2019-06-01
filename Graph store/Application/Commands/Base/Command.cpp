#include "Command.h"
#include "String\String.h"
#include "GraphStore\GraphStore.h"
#include "Graph\Base Graph\Graph.h"
#include "Exceptions\Exceptions.h"
#include "Third party\args\args.hxx"

namespace IDragnev::GraphStore
{
	Graph* Command::usedGraph = nullptr;
	GraphStore* Command::graphStore = nullptr;

	void Command::execute(args::Subparser& parser)
	{
		parseArguments(parser);
		execute();
	}

	void Command::setIfMatched(String& str, StringPositional& argument)
	{
		using args::get;

		if (argument.Matched())
		{
			str = std::move(get(argument));
		}
		else
		{
			throw MissingArgument{ argument.Name() };
		}
	}

	void Command::setManagedStore(GraphStore& store)
	{
		graphStore = &store;
		usedGraph = nullptr;
	}

	void Command::useGraph(const String& ID)
	{
		usedGraph = &getGraph(ID);
	}

	Graph& Command::getGraph(const String& ID)
	{
		assert(graphStore);
		return (*graphStore)[ID];
	}

	Graph& Command::getUsedGraph()
	{
		if (usedGraph)
		{
			return *usedGraph;
		}
		else
		{
			throw Exception{ "No graph is currently being used" };
		}
	}

	void Command::insertGraph(std::unique_ptr<Graph> ptr)
	{
		assert(graphStore);
		graphStore->insert(std::move(ptr));
	}

	void Command::removeGraph(const String& ID)
	{
		assert(graphStore);

		if (usedGraph && usedGraph->getID() == ID)
		{
			usedGraph = nullptr;
		}

		graphStore->remove(ID);
	}
}