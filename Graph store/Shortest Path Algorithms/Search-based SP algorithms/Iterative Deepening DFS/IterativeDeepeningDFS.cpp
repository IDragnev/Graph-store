#include "IterativeDeepeningDFS.h"
#include "..\..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <assert.h>

namespace GS = IDragnev::GraphStore;

static GS::ShortestPathAlgorithmRegistrator<GS::IterativeDeepeningDFS> registrator{ "DFS" };


namespace IDragnev
{
	namespace GraphStore
	{
		ShortestPathAlgorithm::Path
		IterativeDeepeningDFS::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal)
		{
			initState(graph);
			decorate(graph, source);
			findShortestPath(source, goal);
			cleanDecoratedState();

			return std::move(result);
		}

		void IterativeDeepeningDFS::initState(const Graph& graph)
		{
			maxDepth = graph.getVerticesCount() - 1;  //|V| - 1 because shortest paths are unique
			isAShortestPathFound = false;
		}

		void IterativeDeepeningDFS::findShortestPath(const Vertex& source, const Vertex& goal)
		{
			auto& vertex = decoratorOf(source);

			for (auto depthBound = 0U; !isAShortestPathFound && depthBound <= maxDepth; ++depthBound)
			{
				startDepthLimitedSearch(vertex, depthBound);
			}
		}

		void IterativeDeepeningDFS::startDepthLimitedSearch(MarkableVertex& vertex, unsigned depthBound)
		{
			assert(!vertex.isVisited);
			vertex.isVisited = true;

			if (depthBound == 0 && isTheGoal(vertex))
			{
				result = Path{ vertex };
				isAShortestPathFound = true;
			}
			else if (depthBound > 0)
			{
				proceedWithNeighboursOf(vertex, depthBound - 1);
			}

			vertex.isVisited = false;
		}

		void IterativeDeepeningDFS::proceedWithNeighboursOf(const MarkableVertex& vertex, unsigned depthBound)
		{
			auto iteratorPtr = getEdgesLeaving(vertex);

			forEach(*iteratorPtr, [&](const IncidentEdge& edge)
			{
				auto& neighbour = decoratorOf(edge.getIncidentVertex());

				if (!neighbour.isVisited)
				{
					extendCurrentPathFromTo(vertex, neighbour);
					startDepthLimitedSearch(neighbour, depthBound);

					if (isAShortestPathFound)
					{
						return;
					}
				}
			});
		}

		void IterativeDeepeningDFS::extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to)
		{
			to.predecessor = &from;
			to.distance = from.distance + Distance{ 1 };
		}

		void IterativeDeepeningDFS::initSourceDecorator(MarkableVertex& source)
		{
			source.distance = 0;
			source.predecessor = nullptr;
			source.isVisited = false;
		}
	}
}