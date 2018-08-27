#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

class DirectedGraph : public Graph
{
public:
	using Graph::Graph;

	virtual void insertEdge(Vertex& start, Vertex& end, unsigned weight) override;
	virtual void removeEdge(Vertex& start, Vertex& end) override;

private:
	virtual void removeEdgesEndingIn(Vertex& vertex) override;
};

#endif //__DIRECTED_GRAPH_H_INCLUDED__