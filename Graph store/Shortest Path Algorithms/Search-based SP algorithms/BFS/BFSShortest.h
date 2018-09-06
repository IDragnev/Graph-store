#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "..\Base\SearchBasedShortestPathAlgorithm.h"
#include "..\..\..\Queue\Queue.h"

class BFSShortest: public SearchBasedShortestPathAlgorithm
{
private:
	using VertexDecoratorsQueue = Queue<const MarkableVertex*>;
	using Base = SearchBasedShortestPathAlgorithm;

public:
	using SearchBasedShortestPathAlgorithm::SearchBasedShortestPathAlgorithm;

private:
	virtual Path findNonTrivialShortestPath(const Graph& graph, 
											const Vertex& source, 
											const Vertex& goal) override;

	void findShortestPath(const Vertex& source, const Vertex& goal);
	bool isFrontierEmpty() const;
	const MarkableVertex& extractVertexFromFrontier();
	void expandFrontierFrom(const MarkableVertex& v);
	void addToFrontier(const MarkableVertex& v);
	static void extendCurrentPathFromTo(const MarkableVertex& from, MarkableVertex& to);
	void clean();

	virtual void initSourceDecorator(MarkableVertex& source) override;

private:
	VertexDecoratorsQueue queue{};
	Path result{};
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__