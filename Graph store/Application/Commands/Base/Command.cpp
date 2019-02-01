#include "Command.h"
#include "..\..\..\String\String.h"
#include "..\..\..\GraphStore\GraphStore.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\..\..\General Exceptions\Exception.h"

namespace GS = IDragnev::GraphStore;

GS::Graph* GS::Command::usedGraph =  nullptr;
GS::GraphStore* GS::Command::graphStore = nullptr;

namespace IDragnev
{
	namespace GraphStore
	{
		void Command::execute(args::Subparser& parser)
		{
			parseArguments(parser);
			execute();
		}

		void Command::setManagedStore(GraphStore& store)
		{
			graphStore = &store;
			usedGraph = nullptr;
		}

		void Command::useGraph(const String& ID)
		{
			assert(graphStore);

			usedGraph = &graphStore->getGraph(ID);
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
			graphStore->insertGraph(std::move(ptr));
		}

		void Command::removeGraph(const String& ID)
		{
			assert(graphStore);

			if (usedGraph && usedGraph->getID() == ID)
			{
				usedGraph = nullptr;
			}

			graphStore->removeGraph(ID);
		}
	}
}