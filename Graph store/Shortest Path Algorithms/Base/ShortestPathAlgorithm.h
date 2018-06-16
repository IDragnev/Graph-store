#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "../../String/String.h"

class Graph;
class Vertex;

class ShortestPathAlgorithm
{
public:
	virtual ~ShortestPathAlgorithm() = default;

	virtual void findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal) = 0;

	const String& getID() const;

protected:
	ShortestPathAlgorithm(const char* ID);

	static void findTrivialPathFromTo(Vertex& source, Vertex& goal);

	void initializeSingleSource(Graph& graph, Vertex& source) const;
	virtual void initializeVertex(Vertex& vertex) const;
	virtual void initializeSource(Vertex& source) const;

	void registerInstance();

private:
	ShortestPathAlgorithm(ShortestPathAlgorithm&& source) = delete;
	ShortestPathAlgorithm(const ShortestPathAlgorithm& source) = delete;
	ShortestPathAlgorithm& operator=(ShortestPathAlgorithm&& rhs) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm& rhs) = delete;

private:
	const String id;
};

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
