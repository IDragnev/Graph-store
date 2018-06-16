#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public ShortestPathAlgorithm
{
private:
	static IterativeDeepeningDFS theOnlyInstance;

public:
	virtual ~IterativeDeepeningDFS() override = default;

	virtual void findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	IterativeDeepeningDFS();

	void findShortestPathToGoalFrom(Vertex& source);
	void startDepthLimitedSearchFromWithBound(Vertex& vertex, unsigned depthBound);
	void proceedWithNeighboursOfWithBound(Vertex& vertex, unsigned depthBound);
	static void extendCurrentPathFromTo(Vertex& vertexFrom, Vertex& vertexTo);

	bool isTheGoal(const Vertex& vertex) const;

	bool isAShortestPathFound() const;
	void notifyAShortestPathWasFound();
	
	void initializeState(Graph& graph, const Vertex& goal);
	void clearState();

private:
	IterativeDeepeningDFS(IterativeDeepeningDFS&& source) = delete;
	IterativeDeepeningDFS(const IterativeDeepeningDFS& source) = delete;
	IterativeDeepeningDFS& operator=(IterativeDeepeningDFS&& rhs) = delete;
	IterativeDeepeningDFS& operator=(const IterativeDeepeningDFS& rhs) = delete;

private:
	Graph* searchedGraph;
	const Vertex* goal;
	bool isPathFound;
};

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__