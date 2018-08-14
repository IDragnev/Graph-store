#ifndef __UNDIRECTED_GRAPH_H_INCLUDED__
#define __UNDIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

class UndirectedGraph : public Graph
{
public:
	using Graph::Graph;
	virtual ~UndirectedGraph() override = default;

	virtual void insertEdge(Vertex& start, Vertex& end, unsigned weight = 1) override;
	virtual void removeEdge(Vertex& start, Vertex& end) override;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) override;

private:
	void insertEdgeInBothDirections(Vertex& start, Vertex& end, unsigned weight);
};

#endif //__UNDIRECTED_GRAPH_H_INCLUDED__