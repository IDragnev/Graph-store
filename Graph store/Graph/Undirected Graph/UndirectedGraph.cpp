#include "UndirectedGraph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");


UndirectedGraph::UndirectedGraph(const String& ID) :
	Graph(ID)
{
}


void UndirectedGraph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(from, to, weight);
	Graph::insertEdgeFromToWithWeight(to, from, weight);
}


void UndirectedGraph::removeEdgeFromTo(Vertex& from, Vertex& to)
{
	assert(isOwnerOf(from) && isOwnerOf(to));

	Graph::removeEdgeFromTo(from, to);
	Graph::removeEdgeFromTo(to, from);
}


void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	Graph::removeEdgesEndingIn(vertex);
}