#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"
#include "../../Queue/Queue.h"

class BFSShortest: public ShortestPathAlgorithm
{
private:
	static BFSShortest theOnlyInstance;

public:
	virtual ~BFSShortest() override = default;

	virtual void findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	BFSShortest();

	void findShortestPathFrom(Vertex& source);
	void exploreNeighboursOf(Vertex& vertex);
	void processNeighbourDiscoveredFrom(Vertex& neighbour, Vertex& vertexFrom);
	static void extendCurrentPathFromTo(Vertex& vertexFrom, Vertex& vertexTo);

	void addToFrontier(Vertex& vertex);
	Vertex& extractVertexFromTheFrontier();
	bool isFrontierEmpty() const;
	static bool isOnFrontier(const Vertex& vertex);

	bool isTheGoal(const Vertex& vertex) const;

	bool isAShortestPathFound() const;
	void notifyAShortestPathWasFound();

	static void initializeSingleSource(Graph& graph, Vertex& source);

	void initializeState(Graph& graph, const Vertex& goal);
	void clearState();

private:
	BFSShortest(BFSShortest&& source) = delete;
	BFSShortest(const BFSShortest& source) = delete;
	BFSShortest& operator=(BFSShortest&& rhs) = delete;
	BFSShortest& operator=(const BFSShortest& rhs) = delete;

private:
	Graph* searchedGraph;
	const Vertex* goal;
	Queue<Vertex*> verticesQueue;
	bool isPathFound;
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
