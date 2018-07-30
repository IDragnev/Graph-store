#ifndef __BFS_SHORTEST_PATH_H_INCLUDED__
#define __BFS_SHORTEST_PATH_H_INCLUDED__

#include "../Base/ShortestPathAlgorithm.h"
#include "../../Queue/Queue.h"

class BFSShortest: public ShortestPathAlgorithm
{
private:
	typedef ShortestPathAlgorithm Base;

public:
	BFSShortest();
	virtual ~BFSShortest() override = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) override;

private:
	void findShortestPathToGoalFrom(Vertex& source);
	bool isFrontierEmpty() const;
	Vertex& extractVertexFromTheFrontier();
	void expandFrontierFrom(Vertex& vertex);
	void addToFrontier(Vertex& vertex);
	static void extendCurrentPathFromTo(Vertex& from, Vertex& to);

	void initializeState(Graph& graph, const Vertex& goal);
	void clearState();

private:
	BFSShortest(BFSShortest&& source) = delete;
	BFSShortest(const BFSShortest& source) = delete;
	BFSShortest& operator=(BFSShortest&& rhs) = delete;
	BFSShortest& operator=(const BFSShortest& rhs) = delete;

private:
	Queue<Vertex*> verticesQueue;
};

#endif //__BFS_SHORTEST_PATH_H_INCLUDED__
