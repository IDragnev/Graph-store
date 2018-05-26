#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "../../Hash/Hash.h"
#include "../../Dynamic Array/DArray.h"
#include "../../String/String.h"

#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"


class Graph
{
private:
	class KeyAccessor; //TODO: remove this declaration afterwards

	typedef Hash<Vertex, String, KeyAccessor> VertexHashTable;
	typedef DArray<Vertex*> VertexArray;

	typedef DArray<Vertex*>::Iterator VertexIterator;
	typedef DArray<Vertex*>::ConstIterator VertexConstIterator;
	typedef SinglyLinkedList<Edge>::Iterator EdgeIterator;
	typedef SinglyLinkedList<Edge>::ConstIterator EdgeConstIterator;

	typedef ConstIterator<Vertex*> AbstractVertexConstIterator;
	typedef Iterator<Edge> AbstractEdgeIterator;
	typedef ConstIterator<Edge> AbstractEdgeConstIterator;

public:
	virtual ~Graph();

	const String& getID() const;

	void insertVertexWithID(const char* ID);
	void removeVertexWithID(const char* ID);

	virtual void insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, Edge::Weight weight = 1) = 0;
	virtual void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo) = 0;

	Vertex& getVertexWithID(const char* ID);

	unsigned getVerticesCount() const;
	AbstractVertexConstIterator* getIteratorToVertices(); 
	AbstractEdgeIterator* getIteratorToIncidentEdgesOf(Vertex& vertex);  
	AbstractEdgeConstIterator* getConstIteratorToIncidentEdgesOf(const Vertex& vertex) const;  

protected:
	Graph(String ID);

	virtual void removeFromAdjacencyLists(Vertex& vertex) = 0;
	void removeEdgeFromToNoThrow(Vertex& vertexFrom, Vertex& vertexTo);

	bool existsVertexWithID(const char* ID) const;
	bool isFromThisGraph(const Vertex& vertex) const;

private:
	Graph(const Graph& source) = delete;
	Graph(Graph&& source) = delete;
	Graph& operator=(Graph&& rhs) = delete;
	Graph& operator=(const Graph& rhs) = delete;

	void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo, bool throwIfEdgeDoesNotExist);
	static EdgeIterator getEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo);
	
	void insertToVertices(Vertex& vertex);
	void insertToVerticesSearchTable(Vertex& vertex);
	void removeFromVertices(const Vertex& vertex);
	void removeFromVerticesSearchTable(const Vertex& vertex);

	size_t getAppropriateVertexHandle() const;
	Vertex* createVertex(const char* ID) const;
	static void deleteVertex(Vertex* vertex);

	void setID(String ID);

private:
	static const size_t FEWEST_VERTICES_EXPECTED = 32;

private:
	String id;
	VertexArray vertices;
	VertexHashTable verticesSearchTable;
};

#endif // __BASE_GRAPH_H_INCLUDED__