#include "UndirectedGraph.h"

GraphRegistrator<UndirectedGraph> UndirectedGraph::registrator("undirected");


UndirectedGraph::UndirectedGraph(const char* ID) :
	Graph(ID)
{
}


void UndirectedGraph::insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(vertexFrom, vertexTo, weight);
	Graph::insertEdgeFromToWithWeight(vertexTo, vertexFrom, weight);
}


void UndirectedGraph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	Graph::removeEdgeFromTo(vertexFrom, vertexTo);
	Graph::removeEdgeFromTo(vertexTo, vertexFrom);
}


void UndirectedGraph::removeFromAdjacencyLists(Vertex& vertex)
{
	Graph::removeFromAdjacencyLists(vertex);
}