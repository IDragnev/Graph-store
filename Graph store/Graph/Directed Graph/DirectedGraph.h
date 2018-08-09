#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"

class DirectedGraph : public Graph
{
public:
	DirectedGraph(const String& ID);
	virtual ~DirectedGraph() override = default;

	virtual void insertEdge(Vertex& start, Vertex& end, unsigned weight = 1) override;
	virtual void removeEdge(Vertex& start, Vertex& end) override;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) override;

private:
	DirectedGraph(const DirectedGraph&) = delete;
	DirectedGraph(DirectedGraph&&) = delete;
	DirectedGraph& operator=(DirectedGraph&&) = delete;
	DirectedGraph& operator=(const DirectedGraph&) = delete;
};

#endif //__DIRECTED_GRAPH_H_INCLUDED__