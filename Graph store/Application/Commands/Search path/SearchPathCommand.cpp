#include "SearchPathCommand.h"
#include "..\..\..\Graph\Base Graph\Graph.h"
#include "..\Base\StringReader\StringReader.h"
#include "..\..\Command registrator\CommandRegistrator.h"
#include "..\..\..\ShortestPathAlgorithm Store\ShortestPathAlgorithmStore.h"
#include "..\..\..\Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static CommandRegistrator<SearchPathCommand> registrator;

		void SearchPathCommand::parseArguments(args::Subparser& parser)
		{
			auto start = StringPositional{ parser, "startVertexID", "The ID of the start of the sought path" };
			auto end = StringPositional{ parser, "endVertexID", "The ID of the end of the sought path" };
			auto algorithm = StringPositional{ parser, "algorithm", "The algorithm to be used" };
			parser.Parse();

			setIfMatched(startVertexID, start);
			setIfMatched(endVertexID, end);
			setIfMatched(algorithmID, algorithm);
		}

		void SearchPathCommand::execute() const
		{
			auto& graph = Command::getUsedGraph();
			auto& start = graph.getVertex(startVertexID);
			auto& end = graph.getVertex(endVertexID);
			auto& algorithm = ShortestPathAlgorithmStore::instance().getAlgorithm(algorithmID);

			auto path = algorithm.findShortestPath(graph, start, end);
			path.print();
		}

		const char* SearchPathCommand::getName() const noexcept
		{
			return "SEARCH-PATH";
		}

		const char* SearchPathCommand::getDescription() const noexcept
		{
			return "Searches a shortest path between a specified pair of"
				   "vertices in the currently used graph with a specified algorithm";
		}
	}
}