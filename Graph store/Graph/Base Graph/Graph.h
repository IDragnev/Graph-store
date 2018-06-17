#ifndef __BASE_GRAPH_H_INCLUDED__
#define __BASE_GRAPH_H_INCLUDED__

#include "../../Hash/Hash.h"
#include "../../Dynamic Array/DArray.h"
#include "../../String/String.h"

#include "../Vertex/Vertex.h"
#include "../Edge/Edge.h"
#include "../ID Accessor/IDAccessor.h"

#include <memory>

class Graph
{
private:
	typedef Hash<Vertex, String, IDAccessor<Vertex>> VertexHashTable;
	typedef DArray<Vertex*> VertexArray;

	typedef DArray<Vertex*>::Iterator VertexIterator;
	typedef DArray<Vertex*>::ConstIterator VertexConstIterator;
	typedef SinglyLinkedList<Edge>::Iterator EdgeIterator;
	typedef SinglyLinkedList<Edge>::ConstIterator EdgeConstIterator;

protected:
	typedef std::unique_ptr<ConstIterator<Vertex*>> AbstractVertexConstIterator;
	typedef std::unique_ptr<Iterator<Edge>> AbstractEdgeIterator;
	typedef std::unique_ptr<ConstIterator<Edge>> AbstractEdgeConstIterator;
	
public:
	virtual ~Graph();

	void insertVertexWithID(const char* ID);
	void removeVertexWithID(const char* ID);

	virtual void insertEdgeFromToWithWeight(Vertex& vertexFrom, Vertex& vertexTo, Edge::Weight weight = 1) = 0;
	virtual void removeEdgeFromTo(Vertex& vertexFrom, Vertex& vertexTo) = 0;

	Vertex& getVertexWithID(const char* ID);
	const Vertex& getVertexWithID(const char* ID) const;

	unsigned getVerticesCount() const;
	AbstractVertexConstIterator getIteratorToVertices(); 
	AbstractEdgeIterator getIteratorToIncidentEdgesOf(Vertex& vertex);  
	AbstractEdgeConstIterator getConstIteratorToIncidentEdgesOf(const Vertex& vertex) const;  

	const String& getID() const;

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