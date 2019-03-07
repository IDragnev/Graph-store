#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "..\Base\SearchBasedShortestPathAlgorithm.h"
#include "..\..\..\Containers\Fixed size queue\FixedSizeQueue.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class BFSShortest : public SearchBasedShortestPathAlgorithm
		{
		private:
			using Base = SearchBasedShortestPathAlgorithm;
			using DecoratorsPtrQueue = Containers::FixedSizeQueue<const Base::MarkableVertex*>;

		public:
			using Base::SearchBasedShortestPathAlgorithm;

		private:
			Path findNonTrivialShortestPath(const Graph& graph,
											const Vertex& source,
											const Vertex& goal) override;

			Path findShortestPath(const Vertex& source, const Vertex& goal);
			bool isFrontierEmpty() const;
			const MarkableVertex& extractVertexFromFrontier();
			void expandFrontierFrom(const MarkableVertex& v);
			void addToFrontier(const MarkableVertex& v);
			static void extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to);
			
			void clear() override;
			void initSourceDecorator(MarkableVertex& source) override;

		private:
			static constexpr std::size_t INITIAL_QUEUE_SIZE = 32;

		private:
			DecoratorsPtrQueue queue{ INITIAL_QUEUE_SIZE };
		};
	}
}

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
