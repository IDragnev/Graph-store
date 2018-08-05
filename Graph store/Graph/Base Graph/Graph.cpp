#include "Graph.h"
#include "../Graph Exception/GraphException.h"
#include <assert.h>


Graph::Graph(const String& ID) :
	id(),
	vertices(FEWEST_VERTICES_EXPECTED),
	verticesSearchTable(FEWEST_VERTICES_EXPECTED)
{
	setID(ID);
}


void Graph::setID(const String& ID)
{
	if (ID != String(""))
	{
		this->id = ID;
	}
	else
	{
		throw GraphException("A Graph ID must be a valid string");
	}
}


Graph::~Graph()
{
	VertexIterator iterator = vertices.getHeadIterator();

	while (iterator)
	{
		deleteVertex(*iterator);

		++iterator;
	}
}


void Graph::insertVertexWithID(const String& ID)
{
	if (!existsVertexWithID(ID))
	{
		std::unique_ptr<Vertex> newVertexPtr = createVertex(ID);
		insert(*newVertexPtr);

		newVertexPtr.release();
	}
	else
	{
		throw GraphException("A vertex with such ID already exists");
	}
}


void Graph::insert(Vertex& vertex)
{
	insertInVertices(vertex);

	try
	{
		insertInSearchTable(vertex);
	}
	catch (std::bad_alloc&)
	{
		removeFromVertices(vertex);
		throw;
	}
}


void Graph::insertInVertices(Vertex& vertex)
{
	assert(vertex.index == vertices.getCount());

	vertices.insert(&vertex);
}


void Graph::insertInSearchTable(Vertex& vertex)
{
	verticesSearchTable.insert(vertex);
}


void Graph::removeVertexWithID(const String& ID)
{
	Vertex& vertex = getVertexWithID(ID);

	removeEdgesEndingIn(vertex);
	removeFromSearchTable(vertex);
	removeFromVertices(vertex);

	deleteVertex(&vertex);
}


void Graph::removeFromSearchTable(const Vertex& vertex)
{
	//must first be removed from the search table!
	assert(isOwnerOf(vertex));

	verticesSearchTable.remove(vertex.id);
}


void Graph::removeFromVertices(const Vertex& vertexToRemove)
{
	assert(isOwnerOf(vertexToRemove));

	const size_t lastVertexIndex = vertices.getCount() - 1;
	Vertex* lastVertex = vertices[lastVertexIndex];

	lastVertex->index = vertexToRemove.index;
	std::swap(vertices[vertexToRemove.index], vertices[lastVertexIndex]);
	vertices.removeAt(lastVertexIndex);
}


//
//the default implementation is for an undirected graph:
//each of the vertex' neighbours has an edge to it
//
void Graph::removeEdgesEndingIn(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	EdgeIterator iterator = vertex.edges.getHeadIterator();

	while (iterator)
	{
		Edge& currentEdge = *iterator;
		Vertex& neighbour = currentEdge.getIncidentVertex();

		removeEdgeFromToNoThrow(neighbour, vertex);
	}
}


void Graph::removeEdgeFromToNoThrow(Vertex& from, Vertex& to)
{
	removeEdgeFromTo(from, to, false);
}


void Graph::removeEdgeFromTo(Vertex& from, Vertex& to)
{
	removeEdgeFromTo(from, to, true);
}


void Graph::removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist)
{
	assert(isOwnerOf(from) && isOwnerOf(to));

	EdgeIterator iterator = getEdgeFromTo(from, to);

	if (iterator)
	{
		from.edges.removeAt(iterator);
	}
	else if (throwIfEdgeDoesNotExist)
	{
		throw GraphException("No such edge exists");
	}
}


void Graph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight weight)
{
	assert(isOwnerOf(from) && isOwnerOf(to));

	EdgeIterator iterator = getEdgeFromTo(from, to);

	if (!iterator)
	{
		from.edges.insert(Edge(&to, weight));
	}
	else
	{
		throw GraphException("Such edge already exists");
	}
}


Graph::EdgeIterator Graph::getEdgeFromTo(Vertex& from, Vertex& to)
{
	EdgeIterator iterator = from.edges.getHeadIterator();

	while (iterator)
	{
		const Edge& currentEdge = *iterator;

		if (currentEdge.getIncidentVertex() == to)
		{
			break;
		}

		++iterator;
	}

	return iterator;
}


bool Graph::existsVertexWithID(const String& ID) const
{
	return verticesSearchTable.search(ID) != nullptr;
}


bool Graph::isOwnerOf(const Vertex& vertex) const
{
	return (vertex.index < vertices.getCount()) && (vertices[vertex.index] == &vertex);
}


std::unique_ptr<Vertex> Graph::createVertex(const String& ID) const
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


const Vertex& Graph::getVertexWithID(const String& ID) const
{
	const Vertex* result = verticesSearchTable.search(ID);

	if (result)
	{
		return *result;
	}
	else
	{
		throw GraphException("There is no vertex with ID " + ID);
	}
}


Vertex& Graph::getVertexWithID(const String& ID)
{
	return const_cast<Vertex&>( static_cast<const Graph&>(*this).getVertexWithID(ID) );
}


Graph::EdgeAbstractIterator Graph::getIteratorToIncidentEdgesOf(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	EdgeIterator iterator = vertex.edges.getHeadIterator();
	
	return iterator.clone();
}


Graph::EdgeAbstractConstIterator Graph::getConstIteratorToIncidentEdgesOf(const Vertex& vertex) const
{
	assert(isOwnerOf(vertex));

	EdgeConstIterator constIterator = vertex.edges.getHeadConstIterator();

	return constIterator.clone();
}


Graph::VertexAbstractConstIterator Graph::getIteratorToVertices()
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