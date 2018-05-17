#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

class DirectedGraph : public Graph
{
public:
	DirectedGraph(const char* ID);
	virtual ~DirectedGraph() override = default;

	virtual void insertEdgeFromToWithWeight(const char* vertexFromID, const char* vertexToID, Edge::Weight weight = 1) override;
	virtual void removeEdgeFromTo(const char* vertexFromID, const char* vertexToID) override;

protected:
	virtual void removeFromAdjacencyLists(Vertex& vertex) override;

private:
	DirectedGraph(const DirectedGraph& source) = delete;
	DirectedGraph(DirectedGraph&& source) = delete;
	DirectedGraph& operator=(DirectedGraph&& rhs) = delete;
	DirectedGraph& operator=(const DirectedGraph& rhs) = delete;
};

#endif //__DIRECTED_GRAPH_H_INCLUDED__