#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "..\Base\SearchBasedShortestPathAlgorithm.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class IterativeDeepeningDFS : public SearchBasedShortestPathAlgorithm
		{
		private:
			using Depth = decltype(std::declval<Graph>().getVerticesCount());

		public:
			using SearchBasedShortestPathAlgorithm::SearchBasedShortestPathAlgorithm;

		private:
			Path findNonTrivialShortestPath(const Graph& graph,
											const Vertex& source,
											const Vertex& goal) override;

			void findShortestPath(const Vertex& source, const Vertex& goal);
			void startDepthLimitedSearch(MarkableVertex& v, Depth depthBound);
			void proceedWithNeighboursOf(const MarkableVertex& v, Depth depthBound);
			static void extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to);

			void initSourceDecorator(MarkableVertex& source) override;

			void initState(const Graph& graph);

		private:
			Depth maxDepth{};
			Path result{};
			bool isAShortestPathFound = false;
		};
	}
}

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__