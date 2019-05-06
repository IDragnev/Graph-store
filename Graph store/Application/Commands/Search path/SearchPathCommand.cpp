#include "SearchPathCommand.h"
#include "Graph\Base Graph\Graph.h"
#include "Application\Command registrator\CommandRegistrator.h"
#include "ShortestPathAlgorithm Store\ShortestPathAlgorithmStore.h"
#include "Shortest Path Algorithms\Base\ShortestPathAlgorithm.h"
#include "UtilityFunctions.h"

namespace IDragnev::GraphStore
{
	static CommandRegistrator<SearchPathCommand> registrator;

	class PathPrinter
	{
	private:
		using Path = ShortestPathAlgorithm::Path;

	public:
		void operator()(const Path& p) const
		{
			printConstituentVertices(p);
			printLength(p);
		}

	private:
		static void printConstituentVertices(const Path& p);
		static void printLength(const Path& p);
	};

	void PathPrinter::printConstituentVertices(const Path& p)
	{
		auto printVertexId = [](const auto& id) { Utility::print(id, ' '); };
		std::for_each(std::begin(p), std::end(p), printVertexId);
	}

	void PathPrinter::printLength(const Path& p)
	{
		using Utility::print;
		print("\nLength: ", p.getLength(), "\n");
	}

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

	void SearchPathCommand::execute()
	{
		auto& graph = Command::getUsedGraph();
		auto& start = graph.getVertex(startVertexID);
		auto& end = graph.getVertex(endVertexID);
		auto& algorithm = ShortestPathAlgorithmStore::instance().getAlgorithm(algorithmID);

		auto path = algorithm.findShortestPath(graph, start, end);
		PathPrinter{}(path);
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