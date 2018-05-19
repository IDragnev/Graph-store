#include "DirectedGraph.h"
#include <memory>

typedef std::unique_ptr<ConstIterator<Vertex*>> VerticesIteratorPtr;


DirectedGraph::DirectedGraph(const char* ID) :
	Graph(ID)
{
}


void DirectedGraph::insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, Edge::Weight weight)
{
	Graph::insertEdgeFromToWithWeight(vertexFrom, vertexTo, weight);
}


void DirectedGraph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	Graph::removeEdgeFromTo(vertexFrom, vertexTo);
}


void DirectedGraph::removeFromAdjacencyLists(Vertex& vertex)
{
	VerticesIteratorPtr verticesIterator(getIteratorToVertices());

	while (!verticesIterator->isFinished())
	{
		Vertex* currentVertex = verticesIterator->getCurrent();

		removeEdgeFromToNoThrow(*currentVertex, vertex);

		verticesIterator->goToNext();
	}
}