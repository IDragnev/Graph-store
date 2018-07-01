#include "Graph.h"
#include "../Graph Exception/GraphException.h"
#include <assert.h>

Graph::Graph(String ID) :
	id(),
	vertices(FEWEST_VERTICES_EXPECTED),
	verticesSearchTable(FEWEST_VERTICES_EXPECTED)
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
		std::unique_ptr<Vertex> newVertexPtr = createVertex(ID);

		insertToVertices(*newVertexPtr);
		insertToVerticesSearchTable(*newVertexPtr);

		newVertexPtr.release();
	}
	else
	{
		throw GraphException("A vertex with such ID already exists");
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


void Graph::insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, Edge::Weight weight)
{
	assert(isFromThisGraph(vertexFrom) && isFromThisGraph(vertexTo));

	EdgeIterator iteratorToEdge = getEdgeFromTo(vertexFrom, vertexTo);

	if (!iteratorToEdge)
	{
		vertexFrom.edges.insert(Edge(&vertexTo, weight));
	}
	else
	{
		throw GraphException("Such edge already exists");
	}
}


void Graph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	removeEdgeFromTo(vertexFrom, vertexTo, true);
}


void Graph::removeEdgeFromToNoThrow(Vertex& vertexFrom, Vertex& vertexTo)
{
	removeEdgeFromTo(vertexFrom, vertexTo, false);
}


void Graph::removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo, bool throwIfEdgeDoesNotExist)
{
	assert(isFromThisGraph(vertexFrom) && isFromThisGraph(vertexTo));

	EdgeIterator iteratorToEdge = getEdgeFromTo(vertexFrom, vertexTo);

	if (iteratorToEdge)
	{
		vertexFrom.edges.removeAt(iteratorToEdge);
	}
	else if (throwIfEdgeDoesNotExist)
	{
		throw GraphException("No such edge exists");
	}
}


//
//the default implementation is for an undirected graph:
//each of the vertex' neighbours has an edge to it
//
void Graph::removeFromAdjacencyLists(Vertex& vertex)
{
	assert(isFromThisGraph(vertex));

	EdgeIterator adjacentEdgesIterator = vertex.edges.getHeadIterator();

	while (adjacentEdgesIterator)
	{
		Edge& currentEdge = *adjacentEdgesIterator;
		Vertex& neighbour = currentEdge.getIncidentVertex();

		removeEdgeFromToNoThrow(neighbour, vertex);
	}
}


Graph::EdgeIterator Graph::getEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo)
{
	EdgeIterator vertexFromEdgesIterator = vertexFrom.edges.getHeadIterator();

	while (vertexFromEdgesIterator)
	{
		const Edge& currentEdge = *vertexFromEdgesIterator;

		if (currentEdge.getIncidentVertex() == vertexTo)
		{
			break;
		}

		++vertexFromEdgesIterator;
	}

	return vertexFromEdgesIterator;
}


bool Graph::existsVertexWithID(const char* ID) const
{
	return verticesSearchTable.search(ID) != nullptr;
}


bool Graph::isFromThisGraph(const Vertex& vertex) const
{
	return (vertex.index < vertices.getCount()) && (vertices[vertex.index] == &vertex);
}


void Graph::insertToVertices(Vertex& vertex)
{
	assert(vertex.index == vertices.getCount());

	vertices.insert(&vertex);
}


void Graph::insertToVerticesSearchTable(Vertex& vertex)
{
	verticesSearchTable.insert(vertex);
}


void Graph::removeFromVerticesSearchTable(const Vertex& vertex)
{
	//must first be removed from the search table!
	assert(isFromThisGraph(vertex));

	verticesSearchTable.remove(vertex.id);
}


void Graph::removeFromVertices(const Vertex& vertexToRemove)
{
	assert(isFromThisGraph(vertexToRemove));

	const size_t lastVertexIndex = vertices.getCount() - 1;

	Vertex* lastVertex = vertices[lastVertexIndex];
	lastVertex->index = vertexToRemove.index;

	std::swap(vertices[vertexToRemove.index], vertices[lastVertexIndex]);

	vertices.removeAt(lastVertexIndex);
}


std::unique_ptr<Vertex> Graph::createVertex(const char* ID) const
{
	return std::unique_ptr<Vertex>(new Vertex(ID, getAppropriateVertexHandle()));
}


size_t Graph::getAppropriateVertexHandle() const
{
	return vertices.getCount();
}


void Graph::deleteVertex(Vertex* vertex)
{
	delete vertex;
}


const Vertex& Graph::getVertexWithID(const char* ID) const
{
	const Vertex* result = verticesSearchTable.search(ID);

	if (result)
	{
		return *result;
	}
	else
	{
		throw GraphException("No vertex with such ID exists");
	}
}


Vertex& Graph::getVertexWithID(const char* ID)
{
	return const_cast<Vertex&>( static_cast<const Graph&>(*this).getVertexWithID(ID) );
}


Graph::AbstractEdgeIterator Graph::getIteratorToIncidentEdgesOf(Vertex& vertex)
{
	assert(isFromThisGraph(vertex));

	EdgeIterator iteratorToEdges = vertex.edges.getHeadIterator();
	
	return AbstractEdgeIterator(iteratorToEdges.clone());
}


Graph::AbstractEdgeConstIterator Graph::getConstIteratorToIncidentEdgesOf(const Vertex& vertex) const
{
	assert(isFromThisGraph(vertex));

	EdgeConstIterator iteratorToEdges = vertex.edges.getHeadConstIterator();

	return AbstractEdgeConstIterator(iteratorToEdges.clone());
}


Graph::AbstractVertexConstIterator Graph::getIteratorToVertices()
{
	//ConstIterator because the actual pointers are iterated
	VertexConstIterator constIterator = vertices.getHeadConstIterator();

	return AbstractVertexConstIterator(constIterator.clone());
}


const String& Graph::getID() const
{
	return id;
}


unsigned Graph::getVerticesCount() const
{
	return vertices.getCount();
}


void Graph::setID(String ID)
{
	if (ID != String(""))
	{
		this->id = std::move(ID);
	}
	else
	{
		throw GraphException("Graph ID must be a valid string");
	}
}