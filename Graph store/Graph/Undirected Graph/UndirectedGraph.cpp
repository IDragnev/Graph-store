#include "UndirectedGraph.h"
#include "../../Graph Factory/Graph registrator/GraphRegistrator.h"
#include "../../General Exceptions/NoMemoryAvailable.h"

static GraphRegistrator<UndirectedGraph> registrator{ "undirected" };


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
		throw Exception{ "Such edge already exists" };
	}
}


void UndirectedGraph::insertEdgeInBothDirections(Vertex& start, Vertex& end, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(start, end, weight);
	try
	{
		Graph::insertEdgeFromToWithWeight(end, start, weight);
	}
	catch (NoMemoryAvailable&)
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