#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
private:
	typedef ShortestPathAlgorithm Base;

public:
	IterativeDeepeningDFS();
	virtual ~IterativeDeepeningDFS() override = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	void findShortestPathToGoalFrom(Vertex& source);
	void startDepthLimitedSearchFromWithBound(Vertex& vertex, unsigned depthBound);
	void proceedWithNeighboursOfWithBound(Vertex& vertex, unsigned depthBound);
	static void extendCurrentPathFromTo(Vertex& from, Vertex& to);

	void initializeState(Graph& graph, const Vertex& goal);

private:
	IterativeDeepeningDFS(IterativeDeepeningDFS&&) = delete;
	IterativeDeepeningDFS(const IterativeDeepeningDFS&) = delete;
	IterativeDeepeningDFS& operator=(IterativeDeepeningDFS&&) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS&) = delete;

private:
	unsigned maxDepth;
	bool isAShortestPathFound;
};

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__