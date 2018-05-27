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

	static void initializeSingleSource(Graph& graph, Vertex& source);

	/*
	.
	.
	.
	*/

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
