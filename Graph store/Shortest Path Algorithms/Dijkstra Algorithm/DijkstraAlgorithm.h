#ifndef __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
#define __DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"
#include "../../Priority Queue/PriorityQueue.h"
#include "DistanceAccessor.h"
#include "HandleSetter.h"
#include "../../Greater Than/GreaterThan.h"

class DijkstraAlgorithm : public ShortestPathAlgorithm
{
private:
	typedef ShortestPathAlgorithm Base;
	typedef PriorityQueue<Vertex, Distance, DistanceAccessor, GreaterThan<Distance>, HandleSetter> MinPriorityQueue;

public:
	DijkstraAlgorithm(const char* ID);
	virtual ~DijkstraAlgorithm() override = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	void findShortestPathToGoal();
	bool existsVertexWithUndeterminedDistance() const;
	Vertex& closestToSourceFromUndetermined();
	void relaxEdgesLeaving(Vertex& vertex);
	void relaxEdge(Vertex& vertex, Edge& edge);
	void extendCurrentPath(Vertex& vertex, Vertex& neighbour, const Distance& distance);
	void updateDistanceOf(Vertex& vertex, const Distance& distance);

	virtual void initializeVertex(Vertex& vertex) const override;
	virtual void initializeSource(Vertex& source) const override;

	void initializeState(Graph& graph, const Vertex& goal);
	void insertVerticesInPriorityQueue(Graph& graph);
	void clearState();

private:
	DijkstraAlgorithm(DijkstraAlgorithm&&) = delete;
	DijkstraAlgorithm(const DijkstraAlgorithm&) = delete;
	DijkstraAlgorithm& operator=(DijkstraAlgorithm&&) = delete;
	DijkstraAlgorithm& operator=(const DijkstraAlgorithm&) = delete;

private:
	MinPriorityQueue priorityQueue;
};

#endif //__DIJKSTRA_SHORTEST_PATHS_ALG_H_INCLUDED__
