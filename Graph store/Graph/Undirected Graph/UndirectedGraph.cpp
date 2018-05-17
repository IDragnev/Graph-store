#include "UndirectedGraph.h"


UndirectedGraph::UndirectedGraph(const char* ID) :
	Graph(ID)
{
}


void UndirectedGraph::insertEdgeFromToWithWeight(const char* vertexFromID, const char* vertexToID, Edge::Weight weight)
{
	Vertex& vertexFrom = getVertexWithID(vertexFromID);
	Vertex& vertexTo = getVertexWithID(vertexToID);

	Graph::insertEdgeFromToWithWeight(vertexFrom, vertexTo, weight);
	Graph::insertEdgeFromToWithWeight(vertexTo, vertexFrom, weight);
}


void UndirectedGraph::removeEdgeFromTo(const char* vertexFromID, const char* vertexToID)
{
	Vertex& vertexFrom = getVertexWithID(vertexFromID);
	Vertex& vertexTo = getVertexWithID(vertexToID);

	Graph::removeEdgeFromTo(vertexFrom, vertexTo);
	Graph::removeEdgeFromTo(vertexTo, vertexFrom);
}


void UndirectedGraph::removeFromAdjacencyLists(Vertex& vertex)
{
	Graph::removeFromAdjacencyLists(vertex);
}