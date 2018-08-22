#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "../../Hash/Hash.h"
#include "../../Dynamic Array/DArray.h"
#include "../../String/String.h"
#include "../Edge/Edge.h"
#include "../Vertex/Vertex.h"
#include "../ID Accessor/IDAccessor.h"
#include "../../General Exceptions/Exception.h"
#include <memory>

class Graph
{
private:
	using VertexHashTable = Hash<Vertex, String, IDAccessor<Vertex>>;
	using EdgeIterator = SinglyLinkedList<Edge>::iterator;

protected:
	using VertexConstIteratorPtr = std::unique_ptr<ConstIterator<Vertex*>>;
	using EdgeIteratorPtr = std::unique_ptr<Iterator<Edge>>;
	using EdgeConstIteratorPtr = std::unique_ptr<ConstIterator<Edge>>;
	
public:
	Graph(const String& ID);
	virtual ~Graph();

	void insertVertexWithID(const String& ID);
	void removeVertex(const String& ID);
	void removeVertex(Vertex& vertex);

	virtual void insertEdge(Vertex& start, Vertex& end, unsigned weight) = 0;
	virtual void removeEdge(Vertex& start, Vertex& end) = 0;

	Vertex& getVertex(const String& ID);
	const Vertex& getVertex(const String& ID) const;

	unsigned getVerticesCount() const;
	VertexConstIteratorPtr getIteratorToVertices();
	EdgeIteratorPtr getIteratorToEdgesLeaving(Vertex& vertex);
	EdgeConstIteratorPtr getConstIteratorToEdgesLeaving(const Vertex& vertex) const;

	const String& getID() const;

protected:
	virtual void removeEdgesEndingIn(Vertex& vertex) = 0;

	static void removeEdgeFromToNoThrow(Vertex& from, Vertex& to);
	static void removeEdgeFromTo(Vertex& from, Vertex& to);
	static void insertEdgeFromToWithWeight(Vertex& from, Vertex& to, unsigned weight);
	static bool existsEdgeFromTo(Vertex& from, Vertex& to);

	bool hasVertexWithID(const String& ID) const;
	bool isOwnerOf(const Vertex& vertex) const;

private:
	Graph(const Graph&) = delete;
	Graph(Graph&&) = delete;
	Graph& operator=(Graph&&) = delete;
	Graph& operator=(const Graph&) = delete;

	static void removeEdgeFromTo(Vertex& from, Vertex& to, bool throwIfEdgeDoesNotExist);
	static EdgeIterator searchEdgeFromTo(Vertex& from, Vertex& to);

	void tryToInsertVertexWithID(const String& ID);
	void insert(Vertex& vertex);
	void insertInVertices(Vertex& vertex);
	void insertInSearchTable(Vertex& vertex);
	void removeFromVertices(const Vertex& vertex);
	void removeFromSearchTable(const Vertex& vertex);

	std::unique_ptr<Vertex> createVertex(const String& ID) const;
	static void deleteVertex(Vertex* vertex);

	void setID(const String& ID);

private:
	static const size_t FEWEST_VERTICES_EXPECTED = 32;

private:
	String id;
	DArray<Vertex*> vertices;
	VertexHashTable verticesSearchTable;
};

#endif // __BASE_GRAPH_H_INCLUDED__