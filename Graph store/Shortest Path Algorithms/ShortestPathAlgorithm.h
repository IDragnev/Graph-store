#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "../String/String.h"

class Graph;
class Vertex;

class ShortestPathAlgorithm
{
public:
	virtual ~ShortestPathAlgorithm() = default;

	virtual void findShortestPathInGraphFromTo(Graph& G, Vertex& vertexFrom, Vertex& vertexTo) = 0;

	const String& getID() const;

protected:
	ShortestPathAlgorithm(const char* ID);

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
