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

	static void initializeSingleSource(Graph& graph, Vertex& source);

	void findShortestPathToGoal(Vertex& source);
	void findPathWithRestrictedDepth(Vertex* vertex, unsigned depth);

	void initializeState(Graph& graph, const Vertex& goal);
	void clearState();

	bool isTheGoal(const Vertex* vertex) const;

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