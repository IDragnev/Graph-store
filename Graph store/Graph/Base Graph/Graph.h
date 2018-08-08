#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "../../Hash/Hash.h"
#include "../../Dynamic Array/DArray.h"
#include "../../String/String.h"
#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"
#include "../ID Accessor/IDAccessor.h"
#include "../../Exception/Exception.h"
#include <memory>

class Graph;
typedef Exception<Graph> GraphException;

class Graph
{
private:
	typedef Hash<Vertex, String, IDAccessor<Vertex>> VertexHashTable;
	typedef DArray<Vertex*> VertexArray;
	typedef VertexArray::Iterator VertexIterator;
	typedef VertexArray::ConstIterator VertexConstIterator;
	typedef SinglyLinkedList<Edge>::Iterator EdgeIterator;
	typedef SinglyLinkedList<Edge>::ConstIterator EdgeConstIterator;

protected:
	typedef std::unique_ptr<ConstIterator<Vertex*>> VertexAbstractConstIterator;
	typedef std::unique_ptr<Iterator<Edge>> EdgeAbstractIterator;
	typedef std::unique_ptr<ConstIterator<Edge>> EdgeAbstractConstIterator;
	
public:
	virtual ~Graph();

	void insertVertexWithID(const String& ID);
	void removeVertexWithID(const String& ID);

	virtual void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, Edge::Weight weight = 1) = 0;
	virtual void removeEdgeFromTo(Vertex& from, Vertex& to) = 0;

	Vertex& getVertexWithID(const String& ID);
	const Vertex& getVertexWithID(const String& ID) const;

	unsigned getVerticesCount() const;
	VertexAbstractConstIterator getIteratorToVertices();
	EdgeAbstractIterator getIteratorToEdgesLeaving(Vertex& vertex);
	EdgeAbstractConstIterator getConstIteratorToEdgesLeaving(const Vertex& vertex) const;

	const String& getID() const;

protected:
	Graph(const String& ID);

	virtual void removeEdgesEndingIn(Vertex& vertex) = 0;
	void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);

	bool existsVertexWithID(const String& ID) const;
	bool isOwnerOf(const Vertex& vertex) const;

private:
	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;
	Graph& operator=(Graph&&) = delete;
	Graph& operator=(const Graph&) = delete;

	void removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist);
	static void insertEdge(Vertex& from, Vertex& to, Edge::Weight weight);
	static EdgeIterator searchEdgeFromTo(Vertex& from, Vertex& to);
	
	void tryToInsertVertexWithID(const String& ID);
	void insert(Vertex& vertex);
	void insertInVertices(Vertex& vertex);
	void insertInSearchTable(Vertex& vertex);
	void removeFromVertices(const Vertex& vertex);
	void removeFromSearchTable(const Vertex& vertex);

	size_t getAppropriateVertexHandle() const;
	std::unique_ptr<Vertex> createVertex(const String& ID) const;
	static void deleteVertex(Vertex* vertex);

	void setID(const String& ID);

private:
	static const size_t FEWEST_VERTICES_EXPECTED = 32;

private:
	String id;
	VertexArray vertices;
	VertexHashTable verticesSearchTable;
};

#endif // __BASE_GRAPH_H_INCLUDED__