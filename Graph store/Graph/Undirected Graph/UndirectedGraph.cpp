#include "UndirectedGraph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");


UndirectedGraph::UndirectedGraph(const String& ID) :
	Graph(ID)
{
}


void UndirectedGraph::insertEdge(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if (!existsEdgeFromTo(start, end))
	{
		insertEdgeInBothDirections(start, end, weight);
	}
	else
	{
		throw GraphException("Such edge already exists");
	}
}


void UndirectedGraph::insertEdgeInBothDirections(Vertex& start, Vertex& end, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(start, end, weight);
	try
	{
		Graph::insertEdgeFromToWithWeight(end, start, weight);
	}
	catch (GraphException&)
	{
		removeEdgeFromToNoThrow(start, end);
		throw;
	}
}


void UndirectedGraph::removeEdge(Vertex& start, Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	Graph::removeEdgeFromTo(start, end);
	Graph::removeEdgeFromTo(end, start);
}


void UndirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	Graph::removeEdgesEndingIn(vertex);
}