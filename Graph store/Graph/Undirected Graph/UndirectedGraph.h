#ifndef __UNDIRECTED_GRAPH_H_INCLUDED__
#define __UNDIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

class UndirectedGraph : public Graph
{
public:
	UndirectedGraph(const char* ID);
	virtual ~UndirectedGraph() override = default;

	virtual void insertEdgeFromToWithWeight(const char* vertexFromID, const char* vertexToID, Edge::Weight weight = 1) override;
	virtual void removeEdgeFromTo(const char* vertexFromID, const char* vertexToID) override;

protected:
	virtual void removeFromAdjacencyLists(Vertex& vertex) override;

private:
	UndirectedGraph(const UndirectedGraph& source) = delete;
	UndirectedGraph(UndirectedGraph&& source) = delete;
	UndirectedGraph& operator=(UndirectedGraph&& rhs) = delete;
	UndirectedGraph& operator=(const UndirectedGraph& rhs) = delete;
};

#endif //__UNDIRECTED_GRAPH_H_INCLUDED__