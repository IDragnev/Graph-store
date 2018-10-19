#include "SearchPathCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include "..\..\..\ShortestPathAlgorithm Store\ShortestPathAlgorithmStore.h"
#include "..\..\..\Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"
#include "..\MissingArgument exception\MissingArgument.h"

namespace GS = IDragnev::GraphStore;

static GS::CommandRegistrator<GS::SearchPathCommand> registrator;

namespace IDragnev
{
	namespace GraphStore
	{
		void SearchPathCommand::parseArguments(args::Subparser& parser)
		{
			auto startID = StringPositional{ parser, "startVertexID", "The ID of the start of the sought path" };
			auto endID = StringPositional{ parser, "endVertexID", "The ID of the end of the sought path" };
			auto algorithmID = StringPositional{ parser, "algorithm", "The algorithm to be used" };
			parser.Parse();

			setStartVertexID(startID);
			setEndVertexID(endID);
			setAlgorithmID(algorithmID);
		}

		void SearchPathCommand::setStartVertexID(StringPositional& argument)
		{
			setIfMatched(startVertexID, argument);
		}

		void SearchPathCommand::setEndVertexID(StringPositional& argument)
		{
			setIfMatched(endVertexID, argument);
		}

		void SearchPathCommand::setAlgorithmID(StringPositional& argument)
		{
			setIfMatched(algorithmID, argument);
		}

		void SearchPathCommand::setIfMatched(String& str, StringPositional& argument)
		{
			if (argument)
			{
				str = args::get(argument);
			}
			else
			{
				throw MissingArgument{ argument.Name() };
			}
		}

		void SearchPathCommand::execute() const
		{
			auto& usedGraph = Command::getUsedGraph();
			auto& start = usedGraph.getVertex(startVertexID);
			auto& end = usedGraph.getVertex(endVertexID);
			auto& algorithm = ShortestPathAlgorithmStore::instance().getAlgorithm(algorithmID);

			auto path = algorithm.findShortestPath(usedGraph, start, end);
			path.print();
		}

		const char* SearchPathCommand::getName()  const
		{
			return "SEARCH-PATH";
		}

		const char* SearchPathCommand::getDescription() const
		{
			return "Searches a shortest path between a specified pair of"
				   "vertices in the currently used graph with a specified algorithm";
		}
	}
}