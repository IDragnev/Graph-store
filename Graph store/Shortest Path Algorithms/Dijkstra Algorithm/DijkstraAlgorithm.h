#ifndef __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
#define __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"
#include "..\..\Priority Queue\PriorityQueue.h"
#include "..\..\Dynamic Array\DArray.h"
#include "..\..\Hash\Hash.h"
#include <functional>

class DijkstraAlgorithm : public ShortestPathAlgorithm
{
private:
	struct PriorityVertex : VertexDecorator
	{
		using VertexDecorator::VertexDecorator;
		PriorityVertex() : VertexDecorator{ nullptr } {}
		PriorityQueueHandle handle{};
	};
	using PointersCollection = DArray<PriorityVertex*>;

	struct DistanceAccessor
	{
		void operator()(PriorityVertex* v, const Distance& d) const { v->distance = d; }
		const Distance& operator()(const PriorityVertex* v) const { return v->distance; }
	};

	struct HandleSetter
	{
		void operator()(PriorityVertex* v, const PriorityQueueHandle& h) const { v->handle = h; }
	};

	struct IDAccessor
	{
		const String& operator()(const PriorityVertex& v) const { return v.vertex->getID(); }
	};

	using MinPriorityQueue = PriorityQueue<PriorityVertex*, Distance, DistanceAccessor, std::greater<Distance>, HandleSetter>;
	using DecoratorsMap = Hash<PriorityVertex, String, IDAccessor>;
	using DecoratorsArray = DArray<PriorityVertex>;

public:
	using ShortestPathAlgorithm::ShortestPathAlgorithm;

private:
	virtual Path findNonTrivialShortestPath(const Graph& graph, 
											const Vertex& source, 
											const Vertex& goal) override;

	void findShortestPath();
	bool existsVertexWithUndeterminedDistance() const;
	const PriorityVertex& closestToSourceFromUndetermined();
	void relaxEdgesLeaving(const PriorityVertex& v);
	void relaxEdge(const PriorityVertex& v, const Edge& e);
	void extendCurrentPath(const PriorityVertex& from, PriorityVertex& to, const Distance& d);
	void updateDistanceOf(PriorityVertex& v, const Distance& d);

	void decorate(const Graph& g, const Vertex& source);
	void decorate(const Graph& g);
	void buildDecoratorsMap();
	void initSourceDecorator(PriorityVertex& source);
	void buildPriorityQueue();
	PointersCollection decoratorsPointers();
	void clearState();

	PriorityVertex& decoratorOf(const Vertex& v);
	const PriorityVertex& decoratorOf(const Vertex& v) const;

private:
	DecoratorsArray decorators{};
	MinPriorityQueue queue{};
	DecoratorsMap map{};
	Path result{};
};

#endif //__DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
