#ifndef __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
#define __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"
#include "..\..\Containers\Priority Queue\PriorityQueue.h"
#include "..\..\Containers\Dynamic Array\DArray.h"
#include "..\..\Containers\Hash\Hash.h"
#include "..\..\Containers\Hash\HashFunctionStringSpecialization.h"

namespace IDragnev
{
	namespace GraphStore
	{
		class DijkstraAlgorithm : public ShortestPathAlgorithm
		{
		private:
			using Handle = Containers::PriorityQueueHandle;

			struct PriorityVertex : VertexDecorator
			{
				using VertexDecorator::VertexDecorator;
				PriorityVertex() noexcept : VertexDecorator{ nullptr } {}
				Handle handle{};
			};
		
			struct DistanceAccessor
			{
				void operator()(PriorityVertex& v, const Distance& d) const noexcept { v.distance = d; }
				const Distance& operator()(const PriorityVertex& v) const noexcept { return v.distance; }
			};

			struct HandleSetter
			{
				void operator()(PriorityVertex& v, const Handle& h) const noexcept { v.handle = h; }
			};

			struct GreaterThan
			{
				bool operator()(const Distance& lhs, const Distance& rhs) const noexcept { return lhs > rhs; }
			};

			struct IDAccessor
			{
				const String& operator()(const PriorityVertex* v) const noexcept { return v->vertex->getID(); }
			};

			using PriorityVertexRef = std::reference_wrapper<PriorityVertex>;
			using MinPriorityQueue =
				Containers::PriorityQueue<PriorityVertexRef, Distance, DistanceAccessor, GreaterThan, HandleSetter>;
			using DecoratorsMap = Containers::Hash<PriorityVertex*, String, IDAccessor>;
			using DecoratorsArray = Containers::DArray<PriorityVertex>;

		public:
			using ShortestPathAlgorithm::ShortestPathAlgorithm;

		private:
			Path findNonTrivialShortestPath(const Graph& graph,
											const Vertex& source,
											const Vertex& goal) override;

			Path findShortestPath();
			bool existsVertexWithUndeterminedDistance() const;
			const PriorityVertex& closestToSourceFromUndetermined();
			void relaxEdgesLeaving(const PriorityVertex& v);
			void relaxEdge(const PriorityVertex& v, const IncidentEdge& e);
			void extendCurrentPath(const PriorityVertex& from, PriorityVertex& to, const Distance& d);
			void updateDistanceOf(PriorityVertex& v, const Distance& d);

			void decorate(const Graph& g, const Vertex& source);
			void decorateVertices(const Graph& g);
			void buildDecoratorsMap();
			void initSourceDecorator(PriorityVertex& source);
			void buildPriorityQueue();
			void clear() override;

			PriorityVertex& decoratorOf(const Vertex& v);
			const PriorityVertex& decoratorOf(const Vertex& v) const;

		private:
			DecoratorsArray decorators{};
			MinPriorityQueue queue{};
			DecoratorsMap map{};
		};
	}
}
#endif //__DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
