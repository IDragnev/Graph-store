#include "DirectedGraph.h"
#include <memory>

typedef std::unique_ptr<ConstIterator<Vertex*>> VerticesIteratorPtr;


DirectedGraph::DirectedGraph(const char* ID) :
	Graph(ID)
{
}


void DirectedGraph::insertEdgeFromToWithWeight(const char* vertexFromID, const char* vertexToID, Edge::Weight weight)
{
	Vertex& vertexFrom = getVertexWithID(vertexFromID);
	Vertex& vertexTo = getVertexWithID(vertexToID);

	Graph::insertEdgeFromToWithWeight(vertexFrom, vertexTo, weight);
}


void DirectedGraph::removeEdgeFromTo(const char* vertexFromID, const char* vertexToID)
{
	Vertex& vertexFrom = getVertexWithID(vertexFromID);
	Vertex& vertexTo = getVertexWithID(vertexToID);

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