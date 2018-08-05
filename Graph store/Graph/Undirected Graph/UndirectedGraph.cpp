#include "UndirectedGraph.h"

GraphRegistrator<UndirectedGraph> UndirectedGraph::registrator("undirected");


UndirectedGraph::UndirectedGraph(const char* ID) :
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
	Graph::removeEdgeFromTo(from, to);
	Graph::removeEdgeFromTo(to, from);
}


void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	Graph::removeEdgesEndingIn(vertex);
}