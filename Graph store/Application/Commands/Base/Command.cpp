#include "Command.h"
#include "..\..\..\String\String.h"
#include "..\..\..\GraphStore\GraphStore.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "StringReader\StringReader.h"
#include "..\..\..\General Exceptions\Exception.h"
#include "..\..\..\..\Third party\args\args.hxx"

namespace IDragnev
{
	namespace GraphStore
	{
		class MissingArgument : public Exception
		{
		public:
			MissingArgument(const std::string& name) :
				Exception{ "Missing argument: [" + name + "]" } {}
		};

		Graph* Command::usedGraph = nullptr;
		GraphStore* Command::graphStore = nullptr;

		void Command::execute(args::Subparser& parser)
		{
			parseArguments(parser);
			execute();
		}

		void Command::setIfMatched(String& str, StringPositional& argument)
		{
			if (argument.Matched())
			{
				using args::get;
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