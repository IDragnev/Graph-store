#include "DijkstraAlgorithm.h"
#include "..\..\Graph\Base Graph\Graph.h"
#include "..\..\ShortestPathAlgorithm Store\Algorithm registrator\ShortestPathAlgorithmRegistrator.h"
#include <memory>

namespace IDragnev
{
	namespace GraphStore
	{
		static ShortestPathAlgorithmRegistrator<DijkstraAlgorithm> registrator{ "Dijkstra" };

		auto DijkstraAlgorithm::findNonTrivialShortestPath(const Graph& graph, const Vertex& source, const Vertex& goal) -> Path
		{
			decorate(graph, source);
			return findShortestPath();
		}

		void DijkstraAlgorithm::decorate(const Graph& graph, const Vertex& source)
		{
			decorateVertices(graph);
			buildDecoratorsMap();
			initSourceDecorator(decoratorOf(source));
			buildPriorityQueue();
		}

		void DijkstraAlgorithm::decorateVertices(const Graph& graph)
		{
			assert(decorators.isEmpty());
			forEachVertex(graph, [this](const Vertex& v) { decorators.insert({ &v }); });
		}

		void DijkstraAlgorithm::buildDecoratorsMap()
		{
			assert(map.isEmpty());

			for (auto& current : decorators)
			{
				map.insert(&current);
			}
		}

		void DijkstraAlgorithm::initSourceDecorator(PriorityVertex& source)
		{
			assert(queue.isEmpty());
			source.distance = 0;
			source.predecessor = nullptr;
		}

		void DijkstraAlgorithm::buildPriorityQueue()
		{
			assert(queue.isEmpty());
			using std::begin;
			using std::end;
			queue = MinPriorityQueue{ begin(decorators), end(decorators) };
		}

		auto DijkstraAlgorithm::findShortestPath() -> Path
		{
			assert(existsVertexWithUndeterminedDistance());

			while (existsVertexWithUndeterminedDistance())
			{
				auto& vertex = closestToSourceFromUndetermined();

				if (isTheGoal(vertex))
				{
					return Path{ vertex };
				}
				else
				{
					relaxEdgesLeaving(vertex);
				}
			}

			return Path{};
		}

		bool DijkstraAlgorithm::existsVertexWithUndeterminedDistance() const
		{
			return !queue.isEmpty();
		}

		auto DijkstraAlgorithm::closestToSourceFromUndetermined() -> const PriorityVertex&
		{
			return queue.extractOptimal();
		}

		void DijkstraAlgorithm::relaxEdgesLeaving(const PriorityVertex& v)
		{
			forEachIncidentEdgeOf(v, [this, &v](const IncidentEdge& edge) { relaxEdge(v, edge); });
		}

		void DijkstraAlgorithm::relaxEdge(const PriorityVertex& vertex, const IncidentEdge& edge)
		{
			auto& neighbour = decoratorOf(edge.getIncidentVertex());
			auto distanceBetween = Distance{ edge.getWeight() };
			auto distanceTakingThisPath = vertex.distance + distanceBetween;

			if (distanceTakingThisPath < neighbour.distance)
			{
				extendCurrentPath(vertex, neighbour, distanceTakingThisPath);
			}
		}

		void DijkstraAlgorithm::extendCurrentPath(const PriorityVertex& from, PriorityVertex& to, const Distance& d)
		{
			to.predecessor = &from;
			updateDistanceOf(to, d);
		}

		void  DijkstraAlgorithm::updateDistanceOf(PriorityVertex& v, const Distance& d)
		{
			queue.improveKey(v.handle, d);
		}

		void DijkstraAlgorithm::clear()
		{
			map.empty();
			queue.empty();
			decorators.empty();
		}

		auto DijkstraAlgorithm::decoratorOf(const Vertex& v) -> PriorityVertex&
		{
			return const_cast<PriorityVertex&>( static_cast<const DijkstraAlgorithm&>(*this).decoratorOf(v) );
		}

		auto DijkstraAlgorithm::decoratorOf(const Vertex& v) const -> const PriorityVertex&
		{
			auto result = map.search(v.ID());
			assert(result != nullptr);
			return *result;
		}
	}
}