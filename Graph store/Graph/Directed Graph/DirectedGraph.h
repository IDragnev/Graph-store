#ifndef __DIRECTED_GRAPH_H_INCLUDED__
#define __DIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"

class DirectedGraph : public Graph
{
public:
	DirectedGraph(const char* ID);
	virtual ~DirectedGraph() override = default;

	virtual void insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, unsigned weight = 1) override;
	virtual void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo) override;

protected:
	virtual void removeFromAdjacencyLists(Vertex& vertex) override;

private:
	DirectedGraph(const DirectedGraph& source) = delete;
	DirectedGraph(DirectedGraph&& source) = delete;
	DirectedGraph& operator=(DirectedGraph&& rhs) = delete;
	DirectedGraph& operator=(const DirectedGraph& rhs) = delete;

private:
	static GraphRegistrator<DirectedGraph> registrator;
};

#endif //__DIRECTED_GRAPH_H_INCLUDED__