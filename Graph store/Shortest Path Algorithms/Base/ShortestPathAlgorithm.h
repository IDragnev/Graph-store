#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "../../String/String.h"
#include "../../Graph/Base Graph/Graph.h"

class ShortestPathAlgorithm
{
public:
	virtual ~ShortestPathAlgorithm() = default;

	virtual void findShortestPath(Graph& graph, Vertex& source, Vertex& goal) = 0;

	const String& getID() const;

protected:
	ShortestPathAlgorithm(const char* ID);

	static void findTrivialPathFromTo(Vertex& source, Vertex& goal);

	bool isTheGoal(const Vertex& vertex) const;
	
	void initializeSingleSource(Graph& graph, Vertex& source) const;
	virtual void initializeVertex(Vertex& vertex) const;
	virtual void initializeSource(Vertex& source) const;

	std::unique_ptr<Iterator<Edge>> getEdgesLeaving(Vertex& vertex) const;

	void initializeState(Graph& graph, const Vertex& goal);

private:
	ShortestPathAlgorithm(ShortestPathAlgorithm&&) = delete;
	ShortestPathAlgorithm(const ShortestPathAlgorithm&) = delete;
	ShortestPathAlgorithm& operator=(ShortestPathAlgorithm&&) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm&) = delete;

private:
	const String id;
	Graph* searchedGraph;
	const Vertex* goal;
};

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
