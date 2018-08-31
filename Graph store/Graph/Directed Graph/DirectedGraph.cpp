#include "DirectedGraph.h"
#include "..\..\General Exceptions\Exception.h"
#include "..\..\Graph Factory\Graph registrator\GraphRegistrator.h"
#include "..\..\Iterator abstraction\Iterator.h"
#include "..\..\Iterator abstraction\ConstIterator.h"

static GraphRegistrator<DirectedGraph> registrator{ "directed" };


void DirectedGraph::insertEdge(Vertex& start, Vertex& end, unsigned weight)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	if(!existsEdgeFromTo(start, end))
	{
		Graph::insertEdgeFromToWithWeight(start, end, weight);
	}
	else
	{
		throw Exception{ "Such edge already exists" };
	}
}


void DirectedGraph::removeEdge(Vertex& start, Vertex& end)
{
	assert(isOwnerOf(start));
	assert(isOwnerOf(end));

	Graph::removeEdgeFromTo(start, end);
}


void DirectedGraph::removeEdgesEndingIn(Vertex& vertex)
{
	auto iteratorPtr = getIteratorToVertices();

	forEach(*iteratorPtr, [&](Vertex* neighbour)
	{
		removeEdgeFromToNoThrow(*neighbour, vertex);
	});
}