#ifndef __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
#define __SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__

#include "../../String/String.h"
#include "../../Graph/Base Graph/Graph.h"

class ShortestPathAlgorithm
{
public:
	virtual ~ShortestPathAlgorithm() = default;

	virtual void findShortestPathInGraphFromTo(Graph& graph, Vertex& source, Vertex& goal) = 0;

	const String& getID() const;

protected:
	ShortestPathAlgorithm(const char* ID);

	static void findTrivialPathFromTo(Vertex& source, Vertex& goal);

	bool isAShortestPathFound() const;
	void notifyAShortestPathWasFound();
	bool isTheGoal(const Vertex& vertex) const;
	
	void initializeSingleSource(Graph& graph, Vertex& source) const;
	virtual void initializeVertex(Vertex& vertex) const;
	virtual void initializeSource(Vertex& source) const;

	std::unique_ptr<Iterator<Edge>> getIncidentEdgesOf(Vertex& vertex) const;

	void initializeState(Graph& graph, const Vertex& goal);

private:
	ShortestPathAlgorithm(ShortestPathAlgorithm&& source) = delete;
	ShortestPathAlgorithm(const ShortestPathAlgorithm& source) = delete;
	ShortestPathAlgorithm& operator=(ShortestPathAlgorithm&& rhs) = delete;
	ShortestPathAlgorithm& operator=(const ShortestPathAlgorithm& rhs) = delete;

private:
	const String id;

	Graph* searchedGraph;
	const Vertex* goal;
	bool isPathFound;
};

#endif //__SHORTEST_PATH_ALGORITHM_BASE_H_INCLUDED__
