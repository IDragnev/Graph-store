#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"
#include "../../Queue/Queue.h"

class BFS: public ShortestPathAlgorithm
{
public:
	explicit BFS(const char* ID);
	virtual ~BFS() override = default;

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
	BFS(BFS&&) = delete;
	BFS(const BFS&) = delete;
	BFS& operator=(BFS&&) = delete;
	BFS& operator=(const BFS&) = delete;

private:
	Queue<Vertex*> queue;
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
