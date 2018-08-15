#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
public:
	using ShortestPathAlgorithm::ShortestPathAlgorithm;
	virtual ~IterativeDeepeningDFS() override = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	void findShortestPathToGoalFrom(Vertex& source);
	void startDepthLimitedSearchFromWithBound(Vertex& vertex, unsigned depthBound);
	void proceedWithNeighboursOfWithBound(Vertex& vertex, unsigned depthBound);
	static void extendCurrentPathFromTo(Vertex& from, Vertex& to);

	void initializeState(Graph& graph, const Vertex& goal);

private:
	unsigned maxDepth{};
	bool isAShortestPathFound{ false };
};

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__