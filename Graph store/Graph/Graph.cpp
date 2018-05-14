#include "Graph.h"


Graph::Graph(String ID) :
	id(),
	vertices(LEAST_VERTICES_EXPECTED),
	verticesSearchTable(LEAST_VERTICES_EXPECTED)
{
	setID(ID);
}


Graph::~Graph()
{
	VertexIterator verticesIterator = vertices.getHeadIterator();

	while (verticesIterator)
	{
		deleteVertex(*verticesIterator);

		++verticesIterator;
	}
}


void Graph::insertVertexWithID(const char* ID)
{
	if (!existsVertexWithID(ID))
	{
		Vertex* newVertex = createVertex(ID);

		insertToVertices(*newVertex);
		insertToVerticesSearchTable(*newVertex);
	}
	else
	{
		//TODO: Graph exception class?
		throw std::invalid_argument("A vertex with such ID already exists in " + this->id);
	}
}


void Graph::removeVertexWithID(const char* ID)
{
	Vertex& vertexToRemove = getVertexWithID(ID);

	removeFromAdjacencyLists(vertexToRemove);

	removeFromVerticesSearchTable(vertexToRemove);
	removeFromVertices(vertexToRemove);

	deleteVertex(&vertexToRemove);
}


//
//the default implementation is for an unoriented graph:
//each of the vertex' neighbours has an edge to it
//
void Graph::removeFromAdjacencyLists(Vertex& vertex)
{
	EdgeConstIterator adjacentEdgesIterator = vertex.edges.getHeadConstIterator();

	while (adjacentEdgesIterator)
	{
		const Edge& currentEdge = *adjacentEdgesIterator;
		Vertex* neighbour = currentEdge.incidentVertex;

		removeEdgeFromToNoThrow(*neighbour, vertex);
	}
}


void Graph::removeEdgeFromToNoThrow(Vertex& vertexFrom, Vertex& vertexTo)
{
	removeEdgeFromTo(vertexFrom, vertexTo, false);
}


void Graph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	removeEdgeFromTo(vertexFrom, vertexTo, true);
}


void Graph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo, bool throwIfEdgeDoesNotExist)
{
	EdgeIterator iteratorToEdge = getEdgeFromTo(vertexFrom, vertexTo);

	if (iteratorToEdge)
	{
		vertexFrom.edges.removeAt(iteratorToEdge);
	}
	else if (throwIfEdgeDoesNotExist)
	{
		//TODO: Graph exception class?
		throw std::invalid_argument("There is no edge from " + vertexFrom.id + " to " + vertexTo.id);
	}
}


Graph::EdgeIterator Graph::getEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	EdgeIterator vertexFromEdgesIterator = vertexFrom.edges.getHeadIterator();

	while (vertexFromEdgesIterator)
	{
		const Edge& currentEdge = *vertexFromEdgesIterator;

		if (currentEdge.incidentVertex == &vertexTo)
		{
			break;
		}

		++vertexFromEdgesIterator;
	}

	return vertexFromEdgesIterator;
}

