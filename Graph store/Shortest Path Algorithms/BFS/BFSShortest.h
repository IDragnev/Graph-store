#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "..\Base\ShortestPathAlgorithm.h"
#include "..\..\Queue\Queue.h"

class BFSShortest: public ShortestPathAlgorithm
{
public:
	using ShortestPathAlgorithm::ShortestPathAlgorithm;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	void findShortestPathToGoalFrom(Vertex& source);
	bool isFrontierEmpty() const;
	Vertex& extractVertexFromTheFrontier();
	void expandFrontierFrom(Vertex& vertex);
	void addToFrontier(Vertex& vertex);
	static void extendCurrentPathFromTo(Vertex& from, Vertex& to);

	void clearState();

private:
	Queue<Vertex*> queue{};
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
