#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

//members
#include "../Hash/Hash.h"
#include "../Dynamic Array/DArray.h"
#include "../String/String.h"

//nested types
#include "Vertex\Vertex.h"
#include "Edge\Edge.h"


class Graph
{
private:
	class KeyAccessor; //TODO: remove this declaration afterwards

	typedef Hash<Vertex, String, KeyAccessor> VertexHashTable;
	typedef DArray<Vertex*> VertexArray;
	typedef DArray<Vertex*>::Iterator VertexIterator;
	typedef SinglyLinkedList<Edge>::Iterator EdgeIterator;
	typedef size_t VertexHandle;

protected:
	typedef Iterator<Vertex*> AbstractVertexIterator;
	typedef Iterator<Edge> AbstractEdgeIterator;
	typedef ConstIterator<Edge> AbstractEdgeConstIterator;

public:
	virtual ~Graph();

	const String& getID() const;

	AbstractVertexIterator* getVerticesIterator();
	AbstractEdgeIterator* getIncidentEdgesOf(Vertex* vertex);   // OWNERSHIP : VALIDATE THE VERTEX (!)
	AbstractEdgeConstIterator* getIncidentEdgesOf(const Vertex* vertex) const;   // OWNERSHIP : VALIDATE THE VERTEX (!)

	Vertex& getVertexWithID(const char* ID);
	const Vertex& getVertexWithID(const char* ID) const;

	void removeVertex(const char* ID);
	void insertVertex(const char* ID);

	virtual void insertEdgeFromToWithWeight(const char* vertexFromID, const char* vertexToID, Edge::Weight weight = 1) = 0;
	virtual void removeEdgeFromTo(const char* vertexFromID, const char* vertexToID) = 0;

protected:
	Graph(String ID);

	virtual void removeFromAdjacencyLists(Vertex& vertex) = 0;

	static void removeEdgeFromToNoThrow(Vertex& lhs, Vertex& rhs);
	static void removeEdgeFromTo(Vertex& lhs, Vertex& rhs);

	bool existsVertexWithID(const char* ID) const;
	static bool existsAnEdgeFromTo(const Vertex& lhs, const Vertex& rhs);

private:
	Graph(const Graph& source) = delete;
	Graph(Graph&& source) = delete;
	Graph& operator=(Graph&& rhs) = delete;
	Graph& operator=(const Graph& rhs) = delete;

	void setID(String ID);

	void insertToVertices(Vertex& vertex);
	void removeFromVertices(Vertex& vertex);
	void insertToVerticesSearchTable(Vertex& vertex);
	void removeFromVerticesSearchTable(Vertex& vertex);

	VertexHandle getAppropriateVertexHandle() const;

	static Vertex* createVertex(const char* ID);
	static void deleteVertex(Vertex* vertex);

	static EdgeIterator getEdgeFromTo(Vertex& lhs, Vertex& rhs);
	static void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo, bool throwIfEdgeDoesNotExist);

private:
	static const size_t LEAST_VERTICES_EXPECTED = 32;

private:
	String id;
	VertexArray vertices;
	VertexHashTable verticesSearchTable;
};

#endif // __BASE_GRAPH_H_INCLUDED__