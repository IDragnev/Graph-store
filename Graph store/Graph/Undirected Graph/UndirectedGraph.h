#ifndef __UNDIRECTED_GRAPH_H_INCLUDED__
#define __UNDIRECTED_GRAPH_H_INCLUDED__

#include "..\Base Graph\Graph.h"

class UndirectedGraph : public Graph
{
public:
	using Graph::Graph;

	void insertEdge(Vertex& start, Vertex& end, unsigned weight) override;
	void removeEdge(Vertex& start, Vertex& end) override;

private:
	void removeEdgesEndingIn(Vertex& vertex) override;
	void insertEdgeInBothDirections(Vertex& start, Vertex& end, unsigned weight);
};

#endif //__UNDIRECTED_GRAPH_H_INCLUDED__