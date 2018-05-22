#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
private:
	static IterativeDeepeningDFS theOnlyInstance;

public:
	virtual ~IterativeDeepeningDFS() override = default;

	virtual void findShortestPathInGraphFromTo(Graph& G, Vertex& vertexFrom, Vertex& vertexTo) override;

private:
	IterativeDeepeningDFS();

	/*
	.
	.
	.
	*/

private:
	IterativeDeepeningDFS(IterativeDeepeningDFS&& source) = delete;
	IterativeDeepeningDFS(const IterativeDeepeningDFS& source) = delete;
	IterativeDeepeningDFS& operator=(IterativeDeepeningDFS&& rhs) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS& rhs) = delete;

private:
	Graph* searchedGraph;
	Vertex* goal;
	bool isPathFound;
};

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__