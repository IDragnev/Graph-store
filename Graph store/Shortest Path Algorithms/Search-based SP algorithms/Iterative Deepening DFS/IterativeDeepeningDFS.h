#ifndef __ITERATIVE_DEEPENING_DFS_H_INCLUDED__
#define __ITERATIVE_DEEPENING_DFS_H_INCLUDED__

#include "..\Base\SearchBasedShortestPathAlgorithm.h"

class IterativeDeepeningDFS : public SearchBasedShortestPathAlgorithm
{
public:
	using SearchBasedShortestPathAlgorithm::SearchBasedShortestPathAlgorithm;

private:
	virtual Path findNonTrivialShortestPath(const Graph& graph, 
											const Vertex& source, 
											const Vertex& goal) override;

	void findShortestPath(const Vertex& source, const Vertex& goal);
	void startDepthLimitedSearch(MarkableVertex& v, unsigned depthBound);
	void proceedWithNeighboursOf(const MarkableVertex& v, unsigned depthBound);
	static void extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to);

	virtual void initSourceDecorator(MarkableVertex& source) override;

	void initState(const Graph& graph);

private:
	unsigned maxDepth{};
	Path result{};
	bool isAShortestPathFound = false;
};

#endif //__ITERATIVE_DEEPENING_DFS_H_INCLUDED__