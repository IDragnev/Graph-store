#include "BFSShortest.h"
#include "..\..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"

namespace IDragnev
{
	namespace GraphStore
	{
		static ShortestPathAlgorithmRegistrator<BFSShortest> registrator{ "BFS" };

		auto BFSShortest::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal) -> Path
		{
			decorate(graph, source);
			return findShortestPath(source, goal);
		}

		auto BFSShortest::findShortestPath(const Vertex& source, const Vertex& goal) -> Path
		{
			assert(isFrontierEmpty());
			addToFrontier(decoratorOf(source));

			while (!isFrontierEmpty())
			{
				auto& vertex = extractVertexFromFrontier();

				if (isTheGoal(vertex))
				{
					return Path{ vertex };
				}
				else
				{
					expandFrontierFrom(vertex);
				}
			}

			return Path{};
		}

		bool BFSShortest::isFrontierEmpty() const
		{
			return queue.isEmpty();
		}

		void BFSShortest::addToFrontier(const MarkableVertex& v)
		{
			assert(v.isVisited);
			queue.enqueue(&v);
		}

		auto BFSShortest::extractVertexFromFrontier() -> const MarkableVertex&
		{
			auto result = queue.dequeue();
			return *result;
		}

		void BFSShortest::expandFrontierFrom(const MarkableVertex& vertex)
		{
			forEachIncidentEdgeOf(vertex, [this, &vertex](const IncidentEdge& edge)
			{
				auto& neighbour = decoratorOf(edge.getIncidentVertex());

				if (!neighbour.isVisited)
				{
					neighbour.isVisited = true;
					extendCurrentPathFromTo(vertex, neighbour);
					addToFrontier(neighbour);
				}
			});
		}

		void BFSShortest::extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to)
		{
			to.predecessor = &from;
			to.distance = from.distance + Distance{ 1 };
		}

		void BFSShortest::clear()
		{
			queue.empty();
			Base::clear();
		}

		void BFSShortest::initSourceDecorator(MarkableVertex& source)
		{
			Base::initSourceDecorator(source);
		}
	}
}