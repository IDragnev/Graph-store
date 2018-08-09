#include "UndirectedGraph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"

static GraphRegistrator<UndirectedGraph> registrator("undirected");


UndirectedGraph::UndirectedGraph(const String& ID) :
	Graph(ID)
{
}


void UndirectedGraph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, unsigned weight)
{
	assert(isOwnerOf(from) && isOwnerOf(to));

	if (!existsEdgeFromTo(from, to))
	{
		insertEdgeInBothDirections(from, to, weight);
	}
	else
	{
		throw GraphException("Such edge already exists");
	}
}


void UndirectedGraph::insertEdgeInBothDirections(Vertex& from, Vertex& to, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(from, to, weight);
	try
	{
		Graph::insertEdgeFromToWithWeight(to, from, weight);
	}
	catch (GraphException&)
	{
		removeEdgeFromToNoThrow(from, to);
		throw;
	}
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