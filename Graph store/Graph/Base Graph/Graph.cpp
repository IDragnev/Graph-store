#include "Graph.h"
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
		Vertex* newVertex = createVertex(ID);

		insertToVertices(*newVertex);
		insertToVerticesSearchTable(*newVertex);
	}
	else
	{
		//TODO: Graph exception class
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
		//TODO: Graph exception class
		throw std::invalid_argument("There already is an edge from " + vertexFrom.id + " to " + vertexTo.id);
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
		//TODO: Graph exception class
		throw std::invalid_argument("There is no edge from " + vertexFrom.id + " to " + vertexTo.id);
	}
}


//
//the default implementation is for an undirected graph:
//each of the vertex' neighbours has an edge to it
//
void Graph::removeFromAdjacencyLists(Vertex& vertex)
{
	assert(isFromThisGraph(vertex));

	EdgeConstIterator adjacentEdgesIterator = vertex.edges.getHeadConstIterator();

	while (adjacentEdgesIterator)
	{
		const Edge& currentEdge = *adjacentEdgesIterator;
		Vertex* neighbour = currentEdge.incidentVertex;

		removeEdgeFromToNoThrow(*neighbour, vertex);
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


//
//not const becase the search table's search method is not const
//
bool Graph::existsVertexWithID(const char* ID) 
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

	if (vertexToRemove.index != lastVertexIndex)
	{
		std::swap(vertices[vertexToRemove.index], vertices[lastVertexIndex]);

		vertices.remove(lastVertexIndex);

		Vertex* vertexAtItsPlace = vertices[vertexToRemove.index];
		vertexAtItsPlace->index = vertexToRemove.index;
	}
	else
	{
		vertices.remove(vertexToRemove.index);
	}
}


Vertex* Graph::createVertex(const char* ID) const
{
	return new Vertex(ID, getAppropriateVertexHandle());
}


size_t Graph::getAppropriateVertexHandle() const
{
	return vertices.getCount();
}


void Graph::deleteVertex(Vertex* vertex)
{
	delete vertex;
}


Vertex& Graph::getVertexWithID(const char* ID)
{
	Vertex* result = verticesSearchTable.search(ID);

	if (result != nullptr)
	{
		return *result;
	}
	else
	{
		//TODO: Graph exception class
		throw std::invalid_argument("No vertex with ID " + (String)ID + " in " + this->id);
	}
}


Graph::AbstractEdgeIterator* Graph::getIteratorToIncidentEdgesOf(Vertex& vertex)
{
	assert(isFromThisGraph(vertex));

	EdgeIterator iteratorToEdges = vertex.edges.getHeadIterator();
	
	return iteratorToEdges.clone();
}


Graph::AbstractEdgeConstIterator* Graph::getConstIteratorToIncidentEdgesOf(const Vertex& vertex) const
{
	assert(isFromThisGraph(vertex));

	EdgeConstIterator iteratorToEdges = vertex.edges.getHeadConstIterator();

	return iteratorToEdges.clone();
}


Graph::AbstractVertexConstIterator* Graph::getIteratorToVertices()
{
	//ConstIterator because the actual pointers are iterated
	VertexConstIterator constIterator = vertices.getHeadConstIterator();

	return constIterator.clone();
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
		//TODO: Graph exception class 
		throw std::invalid_argument("Graph ID must be a valid string");
	}
}