#ifndef __UNDIRECTED_GRAPH_H_INCLUDED__
#define __UNDIRECTED_GRAPH_H_INCLUDED__

#include "../Base Graph/Graph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"

class UndirectedGraph : public Graph
{
public:
	UndirectedGraph(const char* ID);
	virtual ~UndirectedGraph() override = default;

	virtual void insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, unsigned weight = 1) override;
	virtual void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo) override;

protected:
	virtual void removeFromAdjacencyLists(Vertex& vertex) override;

private:
	UndirectedGraph(const UndirectedGraph&) = delete;
	UndirectedGraph(UndirectedGraph&&) = delete;
	UndirectedGraph& operator=(UndirectedGraph&&) = delete;
	UndirectedGraph& operator=(const UndirectedGraph&) = delete;

private:
	static GraphRegistrator<UndirectedGraph> registrator;
};

#endif //__UNDIRECTED_GRAPH_H_INCLUDED__