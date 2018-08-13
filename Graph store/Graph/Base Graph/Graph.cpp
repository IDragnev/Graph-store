#include "Graph.h"
#include <assert.h>


Graph::Graph(const String& ID) :
	id{},
	vertices{ FEWEST_VERTICES_EXPECTED },
	verticesSearchTable{ FEWEST_VERTICES_EXPECTED }
{
	setID(ID);
}


void Graph::setID(const String& ID)
{
	if (ID != String{ "" })
	{
		this->id = ID;
	}
	else
	{
		throw GraphException{ "A Graph ID must be a valid string" };
	}
}


Graph::~Graph()
{
	VertexConstIterator iterator = vertices.getHeadConstIterator();

	while (iterator)
	{
		deleteVertex(*iterator);
		++iterator;
	}
}


void Graph::deleteVertex(Vertex* vertex)
{
	delete vertex;
}


void Graph::insertVertexWithID(const String& ID)
{
	if (!hasVertexWithID(ID))
	{
		tryToInsertVertexWithID(ID);
	}
	else
	{
		throw GraphException{ "A vertex with such ID already exists" };
	}
}


bool Graph::hasVertexWithID(const String& ID) const
{
	return verticesSearchTable.search(ID) != nullptr;
}


void Graph::tryToInsertVertexWithID(const String& ID)
{
	try
	{
		std::unique_ptr<Vertex> newVertexPtr = createVertex(ID);
		insert(*newVertexPtr);

		newVertexPtr.release();
	}
	catch (std::bad_alloc&)
	{
		throw GraphException{ "No memory available to insert a vertex" };
	}
}


std::unique_ptr<Vertex> Graph::createVertex(const String& ID) const
{
	return std::unique_ptr<Vertex>{new Vertex{ ID, vertices.getCount() }};
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


void Graph::removeFromVertices(const Vertex& vertexToRemove)
{
	assert(isOwnerOf(vertexToRemove));

	const size_t lastVertexIndex = vertices.getCount() - 1;
	Vertex* lastVertex = vertices[lastVertexIndex];

	lastVertex->index = vertexToRemove.index;
	std::swap(vertices[vertexToRemove.index], vertices[lastVertexIndex]);
	vertices.removeAt(lastVertexIndex);
}


void Graph::removeFromSearchTable(const Vertex& vertex)
{
	verticesSearchTable.remove(vertex.id);
}


void Graph::removeVertex(const String& ID)
{
	removeVertex(getVertex(ID));
}


void Graph::removeVertex(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	removeEdgesEndingIn(vertex);
	removeFromSearchTable(vertex);
	removeFromVertices(vertex);

	deleteVertex(&vertex);
}


//
//the default implementation is for an undirected graph:
//each of the vertex' neighbours has an edge to it
//
void Graph::removeEdgesEndingIn(Vertex& vertex)
{
	EdgeIterator iterator = vertex.edges.getHeadIterator();

	while (iterator)
	{
		Edge& edge = *iterator;
		Vertex& neighbour = edge.getIncidentVertex();

		removeEdgeFromToNoThrow(neighbour, vertex);

		++iterator;
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
	EdgeIterator iterator = searchEdgeFromTo(from, to);

	if (iterator)
	{
		from.edges.removeAt(iterator);
	}
	else if (throwIfEdgeDoesNotExist)
	{
		throw GraphException{ "No such edge exists" };
	}
}


Graph::EdgeIterator Graph::searchEdgeFromTo(Vertex& from, Vertex& to)
{
	EdgeIterator iterator = from.edges.getHeadIterator();

	while (iterator)
	{
		const Edge& edge = *iterator;

		if (edge.getIncidentVertex() == to)
		{
			break;
		}

		++iterator;
	}

	return iterator;
}


void Graph::insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight weight)
{
	try
	{
		from.edges.insert(Edge{ &to, weight });
	}
	catch (std::bad_alloc&)
	{
		throw GraphException{ "No memory available to insert an edge" };
	}
}


bool Graph::existsEdgeFromTo(Vertex& from, Vertex& to)
{
	EdgeIterator iterator = searchEdgeFromTo(from, to);

	return !iterator.isFinished();
}


bool Graph::isOwnerOf(const Vertex& vertex) const
{
	return (vertex.index < vertices.getCount()) && (vertices[vertex.index] == &vertex);
}


const Vertex& Graph::getVertex(const String& ID) const
{
	const Vertex* result = verticesSearchTable.search(ID);

	if (result)
	{
		return *result;
	}
	else
	{
		throw GraphException{ "There is no vertex with ID " + ID };
	}
}


Vertex& Graph::getVertex(const String& ID)
{
	return const_cast<Vertex&>( static_cast<const Graph&>(*this).getVertex(ID) );
}


Graph::EdgeAbstractIterator Graph::getIteratorToEdgesLeaving(Vertex& vertex)
{
	assert(isOwnerOf(vertex));

	EdgeIterator iterator = vertex.edges.getHeadIterator();
	
	return iterator.clone();
}


Graph::EdgeAbstractConstIterator Graph::getConstIteratorToEdgesLeaving(const Vertex& vertex) const
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