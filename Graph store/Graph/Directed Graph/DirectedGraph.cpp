#include "DirectedGraph.h"
#include "..\..\Graph Factory\Graph registrator\GraphRegistrator.h"

static GraphRegistrator<DirectedGraph> registrator("directed");


DirectedGraph::DirectedGraph(const String& ID) :
	Graph(ID)
{
}


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
		throw GraphException("Such edge already exists");
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
	VertexAbstractConstIterator iterator(getIteratorToVertices());

	while (!iterator->isFinished())
	{
		Vertex* currentVertex = iterator->getCurrent();
		removeEdgeFromToNoThrow(*currentVertex, vertex);

		iterator->goToNext();
	}
}