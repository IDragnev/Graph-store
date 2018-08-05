#include "DirectedGraph.h"

GraphRegistrator<DirectedGraph> DirectedGraph::registrator("directed");


DirectedGraph::DirectedGraph(const char* ID) :
	Graph(ID)
{
}


void DirectedGraph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, unsigned weight)
{
	Graph::insertEdgeFromToWithWeight(from, to, weight);
}


void DirectedGraph::removeEdgeFromTo(Vertex& from, Vertex& to)
{
	Graph::removeEdgeFromTo(from, to);
}


void DirectedGraph::removeFromAdjacencyLists(Vertex& vertex)
{
	VertexAbstractConstIterator iterator(getIteratorToVertices());

	while (!iterator->isFinished())
	{
		Vertex* currentVertex = iterator->getCurrent();
		removeEdgeFromToNoThrow(*currentVertex, vertex);

		iterator->goToNext();
	}
}